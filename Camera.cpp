#include "Camera.h"
#include "Eigen/src/Core/Matrix.h"
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>

Camera::Camera() : pos(Eigen::Vector3d()), bg_color(Eigen::Vector3d(1.0, 1.0, 1.0)), viewport(Viewport()) {}

Camera::Camera(Eigen::Vector3d pos, double width, double height, double cols, double rows, double viewport_distance, Eigen::Vector3d bg_color)
    : pos(pos), bg_color(bg_color), viewport(Viewport(Eigen::Vector3d(pos.x(), pos.y(), pos.z() - viewport_distance), width, height, cols, rows, viewport_distance)) {}

void Camera::draw_scene(SDL_Renderer* renderer, Cena scene) {
    // Configurar textura e buffer de pixels
    if (!texture || viewport.cols * viewport.rows != static_cast<int>(pixelBuffer.size())) {
        if (texture) SDL_DestroyTexture(texture);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, viewport.cols, viewport.rows);
        pixelBuffer.resize(viewport.cols * viewport.rows);
    }

    // Preencher o buffer com a cor de fundo
    Uint32 bg_pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888),
        static_cast<Uint8>(bg_color.x() * 255),
        static_cast<Uint8>(bg_color.y() * 255),
        static_cast<Uint8>(bg_color.z() * 255),
        255);
    std::fill(pixelBuffer.begin(), pixelBuffer.end(), bg_pixel);

    // Número de threads disponíveis
    unsigned int num_threads = std::thread::hardware_concurrency() * 4;
    std::vector<std::thread> threads;

    // Número total de pixels
    int total_pixels = viewport.rows * viewport.cols;
    int pixels_per_thread = total_pixels / num_threads;

    // Função que cada thread executará
    auto render_chunk = [&](int start, int end) {
        SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
        for (int i = start; i < end; ++i) {
            int row = i / viewport.cols;
            int col = i % viewport.cols;

            Eigen::Vector3d dr = ((viewport.p00 + viewport.dx * col - viewport.dy * row) - pos).normalized();
            Raio r(pos, dr);

            auto [closest_shape, closest_intersect] = scene.get_closest_object(r);

            if (closest_intersect.existe && closest_intersect.t >= 0.0) {
                Eigen::Vector3d p_intersect = r.Po + closest_intersect.t * r.dr;
                Eigen::Vector3d ieye = Eigen::Vector3d::Zero();
                Eigen::Vector3d kd_color;
                if (closest_shape->getMaterial().hasTexture()) {
                    auto [u, v] = closest_shape->getTextureCoords(p_intersect);
                    kd_color = closest_shape->getMaterial().getTextureColor(u, v);
                } else {
                    kd_color = closest_shape->getMaterial().getKd();
                }
                auto [u, v] = closest_shape->getTextureCoords(p_intersect);
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
                    Eigen::Vector3d idif = kd_color.cwiseProduct(light.cor) * nl;
                    Eigen::Vector3d iesp = closest_shape->getMaterial().getKs().cwiseProduct(light.cor) * pow(rv, closest_shape->getMaterial().getShininess());

                    ieye += iamb + idif + iesp;
                }
                if(closest_shape->isSelecioanda()){
                    ieye = Eigen::Vector3d(1.0, 1.0, 0.0);
                }
                draw_pixel(col, row, ieye, format);
            }
        }
        SDL_FreeFormat(format);
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

    updateTexture(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}


void Camera::draw_pixel( int x, int y, Eigen::Vector3d color,SDL_PixelFormat* format) {
    Eigen::Vector3d clamped = color.cwiseMax(0.0).cwiseMin(1.0) * 255.0;
    Uint32 pixel = SDL_MapRGBA(format, clamped.x(), clamped.y(), clamped.z(), 255);
    pixelBuffer[y * viewport.cols + x] = pixel;
}
void Camera::updateTexture(SDL_Renderer* renderer) {
    SDL_UpdateTexture(texture, nullptr, pixelBuffer.data(), viewport.cols * sizeof(Uint32));
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

    forward = Eigen::Vector3d(0, 0, -1);
    right = Eigen::Vector3d(1, 0, 0);
    up = Eigen::Vector3d(0, 1, 0);

    dx = right * (width / cols);
    dy = up * (height / rows);

    top_left = pos - (right * (width / 2.0)) + (up * (height / 2.0));
    p00 = top_left + dx * 0.5 - dy * 0.5;
}
void Camera::Viewport::updateOrientation() {
    dx = right * (width / cols);
    dy = up * (height / rows);
    top_left = pos - (right * (width / 2.0)) + (up * (height / 2.0));
    p00 = top_left + dx * 0.5 - dy * 0.5;
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

void Camera::lookAt(const Eigen::Vector3d& target, const Eigen::Vector3d& upVector) {
    Eigen::Vector3d forward = (target - pos).normalized();
    Eigen::Vector3d right = forward.cross(upVector).normalized();
    Eigen::Vector3d up = right.cross(forward).normalized();

    // Atualizar posição e orientação do viewport
    viewport.pos = pos + forward * viewport.viewport_distance;
    viewport.right = right;
    viewport.up = up;
    viewport.forward = forward;

    // Recalcular dx, dy e pontos do viewport
    viewport.dx = viewport.right * (viewport.width / viewport.cols);
    viewport.dy = viewport.up * (viewport.height / viewport.rows);
    viewport.top_left = viewport.pos - (viewport.right * (viewport.width / 2.0)) + (viewport.up * (viewport.height / 2.0));
    viewport.p00 = viewport.top_left + viewport.dx * 0.5 - viewport.dy * 0.5;
    viewport.pos = pos + viewport.forward * viewport.viewport_distance;
    viewport.updateOrientation();
}

void Camera::zoomIn(double factor) {
    // Reduzindo a distancia do viewport para dar zoom in
    viewport.viewport_distance /= factor;

    // Atualizando o viewport
    viewport.updatePosition(pos);

}

void Camera::zoomOut(double factor) {
    // Aumentando a distancia do viewport para zoom out
    viewport.viewport_distance *= factor;

    // Atualizando o viewport
    viewport.updatePosition(pos);

}
Eigen::Vector3d Camera::rotatePointAroundCamera(const Eigen::Vector3d& target, const Eigen::Vector3d& axis, double degrees) {
    Eigen::Vector3d translatedTarget = target - pos;
    Eigen::AngleAxisd rotation(degrees * M_PI / 180.0, axis.normalized());
    Eigen::Vector3d rotatedTarget = rotation * translatedTarget;
    return pos + rotatedTarget;
}

void Camera::rotateYaw(double degrees) {
    Eigen::Vector3d currentTarget = pos + viewport.forward * viewport.viewport_distance;
    Eigen::Vector3d newTarget = rotatePointAroundCamera(currentTarget, Eigen::Vector3d(0, 1, 0), degrees);
    lookAt(newTarget, Eigen::Vector3d(0, 1, 0));
}

void Camera::rotatePitch(double degrees) {
    Eigen::Vector3d currentTarget = pos + viewport.forward * viewport.viewport_distance;
    Eigen::Vector3d axis = viewport.right.normalized();
    Eigen::Vector3d newTarget = rotatePointAroundCamera(currentTarget, axis, degrees);
    lookAt(newTarget, Eigen::Vector3d(0, 1, 0));
}
