#include "Camera.h"
#include <thread>
#include <mutex>
#include <vector>

Camera::Camera() : pos(Eigen::Vector3d()), bg_color(Eigen::Vector3d(1.0, 1.0, 1.0)), viewport(Viewport()) {}

Camera::Camera(Eigen::Vector3d pos, double width, double height, double cols, double rows, double viewport_distance, Eigen::Vector3d bg_color)
    : pos(pos), bg_color(bg_color), viewport(Viewport(Eigen::Vector3d(pos.x(), pos.y(), pos.z() - viewport_distance), width, height, cols, rows)) {}

// Mutex para proteger o acesso ao renderizador
std::mutex renderMutex;

void Camera::draw_scene(SDL_Renderer* renderer, Cena scene) {
    SDL_SetRenderDrawColor(renderer, bg_color.x(), bg_color.y(), bg_color.z(), 255);
    SDL_RenderClear(renderer);

    // Número de threads disponíveis
    unsigned int num_threads = std::thread::hardware_concurrency() * 3; // Usar 3x o número de threads disponíveis
    std::vector<std::thread> threads;

    // Número total de pixels
    int total_pixels = viewport.rows * viewport.cols;
    int pixels_per_thread = total_pixels / num_threads;

    // Função que cada thread executará
    auto render_chunk = [&](int start_pixel, int end_pixel) {
        for (int pixel = start_pixel; pixel < end_pixel; pixel++) {
            int row = pixel / viewport.cols;
            int col = pixel % viewport.cols;

            Eigen::Vector3d dr = ((viewport.p00 + viewport.dx * col - viewport.dy * row) - pos).normalized();
            Raio r(pos, dr);

            auto [closest_shape, t] = scene.get_closest_object(r);

            if (t > 0.0) {
                Eigen::Vector3d p_intersect = r.Po + t * r.dr;
                Eigen::Vector3d ieye = Eigen::Vector3d::Zero();

                for (Luz light : scene.lights) {
                    bool na_sombra = false;
                    Raio raio_p_luz(p_intersect, light.posicao - p_intersect);

                    for (Forma* s_test : scene.objects) {
                        double distance = s_test->obter_ti(raio_p_luz);
                        if (distance >= 0.0001 && distance <= 1.0) {
                            na_sombra = true;
                            break;
                        }
                    }

                    Eigen::Vector3d l = (light.posicao - p_intersect).normalized();
                    Eigen::Vector3d n = closest_shape->obter_normal(p_intersect);
                    Eigen::Vector3d r = (2.0 * (l.dot(n)) * n - l).normalized();
                    Eigen::Vector3d v = -dr;

                    double nl = n.dot(l);
                    double rv = r.dot(v);
                    if (nl < 0.0 || na_sombra) { nl = 0.0; }
                    if (rv < 0.0 || na_sombra) { rv = 0.0; }

                    Eigen::Vector3d iamb = closest_shape->getMaterial().getKa().cwiseProduct(scene.ambient_light);
                    Eigen::Vector3d idif = closest_shape->getMaterial().getKd().cwiseProduct(light.cor) * nl;
                    Eigen::Vector3d iesp = closest_shape->getMaterial().getKs().cwiseProduct(light.cor) * pow(rv, closest_shape->getMaterial().getShininess());

                    ieye += iamb + idif + iesp;
                }

                // Proteger o acesso ao renderizador com mutex
                std::lock_guard<std::mutex> lock(renderMutex);
                draw_pixel(renderer, col, row, ieye.cwiseMin(1.0).cwiseMax(0.0) * 255.0);
            }
        }
    };

    // Criar threads
    for (unsigned int i = 0; i < num_threads; i++) {
        int start_pixel = i * pixels_per_thread;
        int end_pixel = (i == num_threads - 1) ? total_pixels : start_pixel + pixels_per_thread;
        threads.emplace_back(render_chunk, start_pixel, end_pixel);
    }

    // Aguardar todas as threads terminarem
    for (auto& thread : threads) {
        thread.join();
    }

    SDL_RenderPresent(renderer);
}

void Camera::draw_pixel(SDL_Renderer* renderer, int x, int y, Eigen::Vector3d color) {
    SDL_SetRenderDrawColor(renderer, color.x(), color.y(), color.z(), 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

Camera::Viewport::Viewport() {
    pos = Eigen::Vector3d(0.0, 0.0, -1.0);
    width = 1.0;
    height = 1.0;
    cols = 256;
    rows = 256;

    dx = Eigen::Vector3d(width / cols, 0.0, 0.0);
    dy = Eigen::Vector3d(0.0, height / rows, 0.0);
    top_left = Eigen::Vector3d(pos.x() - width / 2.0, pos.y() + height / 2.0, pos.z());
    p00 = top_left + dx / 2 - dy / 2;
}

Camera::Viewport::Viewport(Eigen::Vector3d pos, double width, double height, double cols, double rows) {
    dx = Eigen::Vector3d(width / cols, 0.0, 0.0);
    dy = Eigen::Vector3d(0.0, height / rows, 0.0);
    top_left = Eigen::Vector3d(pos.x() - width / 2.0, pos.y() + height / 2.0, pos.z());
    p00 = top_left + dx / 2 - dy / 2;

    this->pos = pos;
    this->width = width;
    this->height = height;
    this->cols = cols;
    this->rows = rows;
}

void Camera::Viewport::updatePosition(const Eigen::Vector3d& newPos) {
    pos = newPos;
    top_left = Eigen::Vector3d(pos.x() - width / 2.0, pos.y() + height / 2.0, pos.z());
    p00 = top_left + dx / 2 - dy / 2;
}