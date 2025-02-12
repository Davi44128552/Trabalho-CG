#include <cmath>
#include <chrono>
#include <iostream>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include "Raio.h"
#include "Esfera.h"
#include <algorithm>
#include "Camera.h"
#include "Luz.h"
#include "Cena.h"
#include "Material.h"
#include "Cilindro.h"
using namespace std;
using namespace Eigen;


using namespace std;

int main() {
    Vector3d p0(0,0,10);
    
    double aspect_ratio = 16.0/9.0;
    double viewport_width = 3.2;
    double viewport_height = viewport_width/aspect_ratio;
    double viewport_distance = 1.0;
    int image_width = 960;
    int image_height = image_width/aspect_ratio;

    double sphere_radius = 1.0;
    Vector3d sphere_center(0,0,0);

    Vector3d plane_p0(0.0, -1.8, 0.0);
    Vector3d plane_normal(0.0, 1.0, 0.0);

    Vector3d plane2_p0( 0.0, 0.0, -6.0);
    Vector3d plane2_normal(0.0, 0.0, 1.0);
    
    Vector3d bg_color(0.0, 0.0, 0.0);
    Material mat_sphere = Material(
        Vector3d(0.7, 0.2, 0.2),
        Vector3d(0.7, 0.2, 0.2),
        Vector3d(0.7, 0.2, 0.2),
        10
    );
    Material mat_p1 = Material(
        Vector3d(0.2, 0.7, 0.2),
        Vector3d(0.2, 0.7, 0.2),
        Vector3d(0.0, 0.0, 0.0),
        1
    );
    Material mat_p2 = Material(
        Vector3d(0.3, 0.3, 0.7),
        Vector3d(0.3, 0.3, 0.7),
        Vector3d(0.0, 0.0, 0.0),
        1
    );

    Esfera* sphere = new Esfera(sphere_center, sphere_radius, mat_sphere);
    //Plano* plane = new Plane(plane_p0, plane_normal, mat_p1);
    //Plane* plane2 = new Plane(plane2_p0, plane2_normal, mat_p2);
    Vector3d cylinder_base_center(2.0, 0.0, -(viewport_distance + 2.0 / 2.0));  // Posição do cilindro ao lado da esfera
    Vector3d cylinder_direction(0.0, 1.0, 0.0);
    Cilindro* cilindro = new Cilindro (0.9, 0.5, cylinder_direction, cylinder_base_center, mat_sphere);

    Luz light1 = Luz(
        Vector3d(-0.8, 0.8, 0.0),
        Vector3d(1.0, 0.0, 0.0),
        0.7
    );
    Luz light2 = Luz(
        Vector3d(0.8, 0.8, 0.0),
        Vector3d(0.0, 0.0, 1.0),
        0.7
    );

    Vector3d ambient_light(0.3, 0.3, 0.3);

    Camera camera = Camera(p0, viewport_width, viewport_height, image_width, image_height, viewport_distance, bg_color);

    Vector3d camera_position = p0;
    double camera_speed = 1;

    Cena scene = Cena(ambient_light);
    scene.add_object(sphere);
    //scene.add_object(plane);
    //scene.add_object(plane2);
    scene.add_object(cilindro);
    scene.add_light(light1);
    scene.add_light(light2);

    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { printf("SDL_Init Error: %s\n", SDL_GetError()); return 1; }
    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height, 0);
    if (window == NULL) { printf("SDL_CreateWindow Error: %s\n", SDL_GetError()); SDL_Quit(); return 1; }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { printf("SDL_CreateRenderer Error: %s\n", SDL_GetError()); SDL_DestroyWindow(window); SDL_Quit(); return 1; }

    // contador de fps
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    // main loop
    SDL_Event event;
    while (true) {
        // event handler
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                goto quit;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: camera_position[2] -= 1; break; // Frente
                    case SDLK_s: camera_position[2] += 1; break; // Trás
                    case SDLK_a: camera_position[0] -= 1; break; // Esquerda
                    case SDLK_d: camera_position[0] += 1; break; // Direita
                    case SDLK_q: camera_position[1] += 1; break; // Cima
                    case SDLK_e: camera_position[1] -= 1; break; // Baixo
                }
                
                camera.setPosition(camera_position);
                camera.draw_scene(renderer, scene);
        
            }
        }
        // draw scene
        camera.draw_scene(renderer, scene);

        // printa o FPS no terminal
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        if (elapsedTime.count() >= 1.0) {
            //std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            startTime = currentTime;
        }
    }
    quit:

    delete sphere;
    // SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
