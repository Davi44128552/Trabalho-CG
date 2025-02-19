#include "Camera.h"
#include "Eigen/src/Core/Matrix.h"
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>

Camera::Camera() : pos(Eigen::Vector3d()), bg_color(Eigen::Vector3d(1.0, 1.0, 1.0)), viewport(Viewport()) {}

Camera::Camera(Eigen::Vector3d pos, double width, double height, double cols, double rows, double viewport_distance, Eigen::Vector3d bg_color)
    : pos(pos), bg_color(bg_color), viewport(Viewport(Eigen::Vector3d(pos.x(), pos.y(), pos.z() - viewport_distance), width, height, cols, rows, viewport_distance)) {}

// Mutex para proteger o acesso ao renderizador
std::mutex renderMutex;

void Camera::draw_scene(SDL_Renderer* renderer, Cena* scene) {
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

            auto [closest_shape, closest_intersect] = scene.get_closest_object(r);

            if (closest_intersect.existe && closest_intersect.t >= 0.0) {
                Eigen::Vector3d p_intersect = r.Po + closest_intersect.t * r.dr;
                Eigen::Vector3d ieye = Eigen::Vector3d::Zero();

                for (Luz light : scene.lights) {
                    bool na_sombra = false;
                    Raio raio_p_luz(p_intersect, light.posicao - p_intersect);

                    for (Forma* s_test : scene.objects) {
                        if (s_test == closest_shape) {continue;} // problemas de detecção de colisão consigo mesmo, artefatos escuros no render
                        double distance = s_test->obter_intersecao(raio_p_luz).t;
                        if (distance >= 0.0001 && distance <= 1.0) {
                            na_sombra = true;
                            break;
                        }
                    }

                    Eigen::Vector3d l = (light.posicao - p_intersect).normalized();
                    Eigen::Vector3d n = closest_intersect.normal;
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
                if(closest_shape->isSelecioanda()){
                    ieye = Eigen::Vector3d(1.0, 1.0, 0.0);
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

void Camera::show_context_menu(SDL_Renderer* renderer, int x, int y, Forma* objeto, Cena* scene) {
    bool menu_open = true;
    SDL_Rect menu_box = {x, y, 120, 60};
    SDL_Rect delete_option = {x + 10, y + 10, 100, 20};
    SDL_Rect move_option = {x + 10, y + 35, 100, 20};
    
    SDL_Event event;
    while (menu_open) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                // Se clicar fora do menu, fecha
                if (mx < x || mx > x + 120 || my < y || my > y + 60) {
                    menu_open = false;
                }

                // Excluir objeto
                if (mx > delete_option.x && mx < delete_option.x + 100 &&
                    my > delete_option.y && my < delete_option.y + 20) {
                    scene.remove_object(objeto);
                    menu_open = false;
                }

                
            }
        }

        // Desenhar menu


        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        SDL_RenderFillRect(renderer, &delete_option);
        SDL_RenderPresent(renderer);
        draw_scene(renderer, scene);

    }
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
    viewport_distance = 5.0;

    dx = Eigen::Vector3d(width / cols, 0.0, 0.0);
    dy = Eigen::Vector3d(0.0, height / rows, 0.0);
    top_left = Eigen::Vector3d(pos.x() - width / 2.0, pos.y() + height / 2.0, pos.z());
    p00 = top_left + dx / 2 - dy / 2;
}

Camera::Viewport::Viewport(Eigen::Vector3d pos, double width, double height, double cols, double rows, double viewport_distance) {
    this->pos = pos;
    this->width = width;
    this->height = height;
    this->cols = cols;
    this->rows = rows;
    this->viewport_distance = viewport_distance;

    dx = Eigen::Vector3d(width / cols, 0.0, 0.0);
    dy = Eigen::Vector3d(0.0, height / rows, 0.0);
    top_left = Eigen::Vector3d(pos.x() - width / 2.0, pos.y() + height / 2.0, pos.z());
    p00 = top_left + dx / 2 - dy / 2;
}

void Camera::Viewport::updatePosition(const Eigen::Vector3d& newPos) {
    // Atualiza a posição do viewport
    Eigen::Vector3d previous_pos = this->pos;
    pos = newPos;
    pos[2] -= viewport_distance;
    std::cout << "Atualizando posição de: " << previous_pos.transpose() << " para: " << pos.transpose() << std::endl;

    dx = Eigen::Vector3d(width / cols, 0.0, 0.0);
    dy = Eigen::Vector3d(0.0, height / rows, 0.0);
    top_left = Eigen::Vector3d(pos.x() - width / 2.0, pos.y() + height / 2.0, pos.z());
    p00 = top_left + dx / 2 - dy / 2;

}