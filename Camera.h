#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include "Cena.h"
#include "Raio.h"

class Camera {
public:
    Eigen::Vector3d pos, bg_color;

    Camera();
    Camera(Eigen::Vector3d pos, double width, double height, double cols, double rows, double viewport_distance, Eigen::Vector3d bg_color);

    void draw_scene(SDL_Renderer* renderer, Cena scene);
    void setPosition(const Eigen::Vector3d& newPos) {
        pos = newPos;
        viewport.updatePosition(newPos);
    }
    Eigen::Vector3d getViewportPosition() const { return viewport.pos; }

private:
    class Viewport {
    public:
        Eigen::Vector3d pos, dx, dy, top_left, p00;
        double width, height;
        int cols, rows;

        Viewport();
        Viewport(Eigen::Vector3d pos, double width, double height, double cols, double rows);
        void updatePosition(const Eigen::Vector3d& newPos);
    };

    Viewport viewport;

    inline void draw_pixel(SDL_Renderer* renderer, int x, int y, Eigen::Vector3d color);
};

#endif // CAMERA_H