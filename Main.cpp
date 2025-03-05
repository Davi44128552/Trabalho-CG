#include <cmath>
#include <chrono>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Cone.h"
#include "Esfera.h"
#include "Camera.h"
#include "Luz.h"
#include "Cena.h"
#include "Material.h"

using namespace std;
using namespace Eigen;


using namespace std;

int main() {
    Vector3d p0(0,0,10);
    
    double aspect_ratio = 16.0/9.0;
    double viewport_width = 3.2;
    double viewport_height = viewport_width/aspect_ratio;
    double viewport_distance = 5.0;
    int image_width = 960;
    int image_height = image_width/aspect_ratio;

        // SDL init
        if (SDL_Init(SDL_INIT_VIDEO) != 0) { printf("SDL_Init Error: %s\n", SDL_GetError()); return 1; }
        SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height, 0);
        if (window == NULL) { printf("SDL_CreateWindow Error: %s\n", SDL_GetError()); SDL_Quit(); return 1; }
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) { printf("SDL_CreateRenderer Error: %s\n", SDL_GetError()); SDL_DestroyWindow(window); SDL_Quit(); return 1; }
            // Inicialize a SDL_image (para carregar PNG)
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Erro ao inicializar SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
   // Carregue a imagem PNG
   SDL_Surface* surface = IMG_Load("textura_blocos.jpg");
   if (!surface) {
       std::cerr << "Erro ao carregar a imagem: " << IMG_GetError() << std::endl;
       IMG_Quit();
       SDL_Quit();
       return 1;
   }

   // Extraia os dados da superfície
   uint32_t width = surface->w;
   uint32_t height = surface->h;
   uint32_t pitch = surface->pitch;
   uint32_t bpp = surface->format->BytesPerPixel;

   // Copie os dados da superfície para um vetor
   uint8_t* pixels = static_cast<uint8_t*>(surface->pixels);
   std::vector<uint8_t> textureData(pixels, pixels + (height * pitch));
   if (surface->format->BytesPerPixel < 3) {
    std::cerr << "Formato de imagem não suportado!" << std::endl;
    SDL_FreeSurface(surface);
    IMG_Quit();
    SDL_Quit();
    return 1;
}

   // Crie a textura
   Texture* texture = new Texture(textureData, width, height, pitch, bpp);

    double sphere_radius = 0.5;
    Vector3d sphere_center(0,-0.5,0);

   /*  Vector3d plane_p0(0.0, -2.0, 0.0);
    Vector3d plane_normal(0.0, 1.0, 0.0);

    Vector3d plane2_p0( 0.0, 0.0, -6.0);
    Vector3d plane2_normal(0.0, 0.0, 1.0); */
    Vector3d bg_color(0.0, 0.0, 0.0);
    Material mat_sphere = Material(
        Vector3d(0.0, 0.0, 0.0), // ka (reflexão ambiente)
        Vector3d(1.0, 1.0, 1.0), // kd (reflexão difusa) - branco para textura pura
        Vector3d(0.7, 0.2, 0.2), // ks (reflexão especular)
        4,                      // shininess (expoente de brilho)
        texture 
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
    
    
/*     // Adicao do cubo
    // Definição dos vértices do cubo (agora deslocado para a esquerda)
std::vector<Eigen::Vector3d> vertices = {
    Eigen::Vector3d(-3,  1, -5), Eigen::Vector3d(-1,  1, -5),  // Superiores frontais
    Eigen::Vector3d(-1, -1, -5), Eigen::Vector3d(-3, -1, -5),  // Inferiores frontais
    Eigen::Vector3d(-3,  1, -7), Eigen::Vector3d(-1,  1, -7),  // Superiores traseiros
    Eigen::Vector3d(-1, -1, -7), Eigen::Vector3d(-3, -1, -7)   // Inferiores traseiros
};
 */
// Definição das faces do cubo
/* std::vector<Triangulo> faces = {
    // Frente
    Triangulo(vertices[0], vertices[1], vertices[2]),
    Triangulo(vertices[0], vertices[2], vertices[3]),

    // Trás
    Triangulo(vertices[4], vertices[5], vertices[6]),
    Triangulo(vertices[4], vertices[6], vertices[7]),

    // Esquerda
    Triangulo(vertices[0], vertices[4], vertices[7]),
    Triangulo(vertices[0], vertices[7], vertices[3]),

    // Direita
    Triangulo(vertices[1], vertices[5], vertices[6]),
    Triangulo(vertices[1], vertices[6], vertices[2]),

    // Topo
    Triangulo(vertices[0], vertices[1], vertices[5]),
    Triangulo(vertices[0], vertices[5], vertices[4]),

    // Base
    Triangulo(vertices[3], vertices[2], vertices[6]),
    Triangulo(vertices[3], vertices[6], vertices[7])
};

    Malha* malha = new Malha(faces, vertices, mat_sphere);
    // malha->translacao(Eigen::Vector3d(3, 3, 3));
     */
    
    Esfera* sphere = new Esfera(sphere_center, sphere_radius, mat_sphere);
    Esfera* sphere2 = new Esfera(Vector3d(1.5, 0.0, 0.0), 0.5, mat_p1);
    Esfera* sphere3 = new Esfera(Vector3d(0.0, 0.0, 0.0), 0.5, mat_p2);
    Esfera* sphere4 = new Esfera(Vector3d(-1.5, 0.0, 0.0), 0.5, mat_sphere);
/*     
    Plano* plane = new Plano(plane_p0, plane_normal, mat_p1);
    Plano* plane2 = new Plano(plane2_p0, plane2_normal, mat_p2);
     */
/*     Vector3d cylinder_base_center(1.375, -2.0, -(viewport_distance + 2.0 / 2.0));  // Posição do cilindro ao lado da esfera
    Vector3d cylinder_direction(0.0, 1.0, 1.0);
    Cilindro* cilindro = new Cilindro (2.5, 0.5, cylinder_direction, cylinder_base_center, mat_sphere);

    Vector3d cone_base_center(3.0, -2.0, -(viewport_distance + 2.0 / 2.0));
    Vector3d cone_direction(0.0, 1.0, 1.0);
    Cone* cone = new Cone (1.5, 1.0, cone_direction, cone_base_center, mat_sphere);
 */

    Luz light1 = Luz(
        Vector3d(0.0, 6.0, 11.0),
        Vector3d(1.0, 1.0, 1.0),
        0.6
    );
    Luz light2 = Luz(
        Vector3d(0.0, 7000.0, 110000.0),
        Vector3d(1.0, 1.0, 1.0),
        0.6
    );

    Vector3d ambient_light(0.3, 0.3, 0.3);

    Camera camera = Camera(p0, viewport_width, viewport_height, image_width, image_height, viewport_distance, bg_color);

    Vector3d camera_position = p0;
    double camera_speed = 1;

    Cena scene = Cena(ambient_light);
    //scene.add_object(malha);
    scene.add_object(sphere);
/*  scene.add_object(plane);
    scene.add_object(plane);

    scene.add_object(plane2);
    scene.add_object(cilindro);
    scene.add_object(cone); */
    scene.add_light(light1);
    scene.add_light(light2);
    scene.add_object(sphere2);
    scene.add_object(sphere3);
    scene.add_object(sphere4);

    // contador de fps
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    // main loop
    SDL_Event event;
    camera.draw_scene(renderer, scene);
    while (true) {
        // event handler
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                goto quit;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: camera_position[2] -= 0.5; break; // Frente
                    case SDLK_s: camera_position[2] += 0.5; break; // Trás
                    case SDLK_a: camera_position[0] -= 0.5; break; // Esquerda
                    case SDLK_d: camera_position[0] += 0.5; break; // Direita
                    case SDLK_q: camera_position[1] += 0.5; break; // Cima
                    case SDLK_e: camera_position[1] -= 0.5; break; // Baixo

                    // Funcao para aplicar zoom in e zoom out, conforte e requisitado
                    case SDLK_o: camera.zoomIn(1.1); break; // zoom in
                    case SDLK_i: camera.zoomOut(1.1); break; // zoom out


                    case SDLK_f: malha->translacao(Eigen::Vector3d(1, 1, 1)); break;
                    case SDLK_g: malha->rotacionar_quaternio(45, Eigen::Vector3d(1, 1, 1)); break;
                    case SDLK_h: malha->rotacionar_eixo('x', 30); break;
                    case SDLK_j: malha->escalonar(Eigen::Vector3d(1.1, 1.1, 1.1)); break;
                    case SDLK_k: malha->cisalhar(0, 1, 0, 0, 0, 0); break;

                    case SDLK_DELETE: {
                        // Procurar o objeto selecionado
                        Forma* selecionado = nullptr;
                        for (Forma* obj : scene.objects) {
                            if (obj->isSelecioanda()) {
                                selecionado = obj;
                                break;
                            }
                        }
                        // Se encontrou, remover e redesenhar
                        if (selecionado) {
                            if (scene.remove_object(selecionado)) {
                                camera.draw_scene(renderer, scene);
                            }
                        }
                        break;
                    }
                }

                camera.setPosition(camera_position);

            }else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                Eigen::Vector3d dr = ((camera.getViewport().p00 + camera.getViewport().dx * mouseX - camera.getViewport().dy * mouseY) - camera.pos).normalized();
                // Raio gerado a partir do clique do mouse (ponto de origem e direção)
                Raio raioCLique(camera.pos, dr);
                auto [forma_selecionada, t] = scene.get_closest_object(raioCLique);
                Eigen::Vector3d pontoAlvo = raioCLique.Po + t.t * raioCLique.dr;

                // Atualiza a câmera para olhar para o ponto clicado
                camera.lookAt(pontoAlvo, Eigen::Vector3d(0, 1, 0));

                // Re-renderizar cena
                camera.draw_scene(renderer, scene);
                if (forma_selecionada) {
                    // Desmarcar todas as formas
                    for (Forma* obj : scene.objects) {
                        obj->setSelecionada(false);
                    }
                    
                    // Marcar forma clicada
                    forma_selecionada->setSelecionada(true);
                   
                    // Re-renderizar cena
                    camera.draw_scene(renderer, scene);
                }

            }}
        // draw scene
        camera.draw_scene(renderer, scene);

        // printa o FPS no terminal
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        if (elapsedTime.count() >= 1.0) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            startTime = currentTime;
        }
    }
    quit:

    // SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
