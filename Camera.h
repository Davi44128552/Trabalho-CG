#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include "Cena.h"
#include "Raio.h"

class Camera {
public:
    class Viewport {
    public:
        Eigen::Vector3d pos, dx, dy, top_left, p00;
        double width, height;
        double viewport_distance;
        int cols, rows;
        Eigen::Vector3d right;
        Eigen::Vector3d up;
        Eigen::Vector3d forward;

        Viewport();
        Viewport(Eigen::Vector3d pos, double width, double height, double cols, double rows, double viewport_distance);
        void updatePosition(const Eigen::Vector3d& newPos);
        void updateOrientation();
    };
    Eigen::Vector3d pos, bg_color;

    Camera();
    Camera(Eigen::Vector3d pos, double width, double height, double cols, double rows, double viewport_distance, Eigen::Vector3d bg_color);

    void draw_scene(SDL_Renderer* renderer, Cena scene);
    void setPosition(const Eigen::Vector3d& newPos) {
        this->pos = newPos;
            
        viewport.pos = pos + viewport.forward * viewport.viewport_distance;
        viewport.updateOrientation();
    }
    Eigen::Vector3d getPosition() const { return pos; }
    void lookAt(const Eigen::Vector3d& target, const Eigen::Vector3d& upVector);
    Eigen::Vector3d getViewportPosition() const { return viewport.pos; }
    const Viewport& getViewport() const { return viewport; }

    // Funcoes para realizar zoom in e zoom out
    void zoomIn(double factor);
    void zoomOut(double factor);
    void rotateYaw(double degrees);
    void rotatePitch(double degrees);


private:

    Viewport viewport;

    inline void draw_pixel(SDL_Renderer* renderer, int x, int y, Eigen::Vector3d color);
    Eigen::Vector3d rotatePointAroundCamera(const Eigen::Vector3d& target, const Eigen::Vector3d& axis, double degrees);
};

#endif // CAMERA_H
