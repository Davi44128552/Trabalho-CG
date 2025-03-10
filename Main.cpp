#include <cmath>
#include <chrono>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <eigen3/Eigen/src/Core/Matrix.h>

#include "Cone.h"
#include "Esfera.h"
#include "Camera.h"
#include "Luz.h"
#include "Cena.h"
#include "Material.h"
#include "Plano.h"
#include "Cilindro.h"
#include "Triangulo.h"
#include "Malha.h"
#include "Texture.h"
#include "Cone.h"
#include "TextureLoader.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

using namespace std;
using namespace Eigen;


using namespace std;

int main() {
    Vector3d p0(5,1,5);
    
    double aspect_ratio = 16.0/9.0;
    double viewport_width = 0.25;
    double viewport_height = viewport_width/aspect_ratio;
    double viewport_distance = 0.25;
    int image_width = 960;
    int image_height = image_width/aspect_ratio;

    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { printf("SDL_Init Error: %s\n", SDL_GetError()); return 1; }
    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height, 0);
    if (window == NULL) { printf("SDL_CreateWindow Error: %s\n", SDL_GetError()); SDL_Quit(); return 1; }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { printf("SDL_CreateRenderer Error: %s\n", SDL_GetError()); SDL_DestroyWindow(window); SDL_Quit(); return 1; }
    // para carregar PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Erro ao inicializar SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }


    Texture* texturaMadeira = TextureLoader::loadTexture("textura_madeira.jpg");
    Texture* texturaMarmore = TextureLoader::loadTexture("textura_mar.jpg");
    Texture* texturaPlastico = TextureLoader::loadTexture("textura_plastico.jpg");
    Texture* texturaTijolo = TextureLoader::loadTexture("textura_blocos.jpg");
    Texture* texturaMetal = TextureLoader::loadTexture("textura_metal.jpg");
    Texture* texturaTijoloBranco = TextureLoader::loadTexture("textura_tijolo_branco.jpg");
    Texture* texturaMadeiraWall = TextureLoader::loadTexture("textura_wall.jpg");
    Texture* texturaPiso = TextureLoader::loadTexture("textura_piso.jpg");
    Texture* texturaBola1 = TextureLoader::loadTexture("Ball1.jpg");
    Texture* texturaBola2 = TextureLoader::loadTexture("Ball2.jpg");
    Texture* texturaBola3 = TextureLoader::loadTexture("Ball3.jpg");
    Texture* texturaBola4 = TextureLoader::loadTexture("Ball4.jpg");
    Texture* texturaBola5 = TextureLoader::loadTexture("Ball5.jpg");

    Material mat_default(
        Vector3d(0.7, 0.1, 0.1),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(0.6, 0.4, 0.2),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.6, 0.4, 0.2),   // ks (brilho baixo, já que madeira não reflete muito)
        5                      // shininess (superfície áspera, brilho baixo)
    );

    Material mat_madeira(
        Vector3d(0.2, 0.1, 0.05),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(0.6, 0.4, 0.2),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.3, 0.2, 0.1),   // ks (brilho baixo, já que madeira não reflete muito)
        10,                        // shininess (superfície áspera, brilho baixo)
        texturaMadeira
    );

    Material mat_marmore(
        Vector3d(0.3, 0.3, 0.3),   // ka (mármore reflete um pouco mais a luz ambiente)
        Vector3d(0.7, 0.7, 0.7),   // kd (superfície mais brilhante e reflexiva)
        Vector3d(0.5, 0.5, 0.5),   // ks (reflexão especular moderada)
        50,                        // shininess (superfície relativamente lisa)
        texturaMarmore
    );

    Material mat_plastico(
        Vector3d(0.2, 0.2, 0.2),   // ka (baixa reflexão ambiente)
        Vector3d(0.8, 0.2, 0.2),   // kd (cor mais forte e difusa)
        Vector3d(0.6, 0.6, 0.6),   // ks (plástico tem brilho considerável)
        60,                        // shininess (plástico geralmente tem brilho alto)
        texturaPlastico
    );

    Material mat_tijolo(
        Vector3d(0, 0, 0),   // ka (tijolo reflete pouco a luz ambiente)
        Vector3d(1, 1, 1),   // kd (bastante difuso, já que tijolos não refletem muito)
        Vector3d(0.1, 0.1, 0.1),   // ks (quase sem brilho, textura rugosa)
        1,                         // shininess (superfície muito áspera)
        texturaTijolo
    );

    Material mat_tijolo_branco(
        Vector3d(0, 0, 0),   // ka (tijolo reflete pouco a luz ambiente)
        Vector3d(1, 1, 1),   // kd (bastante difuso, já que tijolos não refletem muito)
        Vector3d(0.1, 0.1, 0.1),   // ks (quase sem brilho, textura rugosa)
        5,                       // shininess (superfície muito áspera)
        texturaTijoloBranco
    );
    
    Material mat_metal(
        Vector3d(0.4, 0.4, 0.4),   // ka (reflete bastante luz ambiente)
        Vector3d(0.8, 0.8, 0.8),   // kd (superfície bem reflexiva)
        Vector3d(1.0, 1.0, 1.0),   // ks (reflexão especular intensa)
        100,                       // shininess (metal polido tem brilho muito alto)
        texturaMetal
    );
    Material mat_wall_madeira(
        Vector3d(0, 0, 0),   // ka (tijolo reflete pouco a luz ambiente)
        Vector3d(1, 1, 1),   // kd (bastante difuso, já que tijolos não refletem muito)
        Vector3d(0.1, 0.1, 0.1),   // ks (quase sem brilho, textura rugosa)
        10,                        // shininess (superfície áspera, brilho baixo)
        texturaMadeiraWall
    );

    Material mat_piso(
        Vector3d(0.2, 0.1, 0.05),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(0.6, 0.4, 0.2),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.3, 0.2, 0.1),   // ks (brilho baixo, já que madeira não reflete muito)
        10,                        // shininess (superfície áspera, brilho baixo)
        texturaPiso
    );

    Material mat_bola1(
        Vector3d(0.2, 0.1, 0.05),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(1,1,1),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.3, 0.2, 0.1),   // ks (brilho baixo, já que madeira não reflete muito)
        50,                        // shininess (superfície áspera, brilho baixo)
        texturaBola1
    );
    Material mat_bola2(
        Vector3d(0.2, 0.1, 0.05),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(1,1,1),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.3, 0.2, 0.1),   // ks (brilho baixo, já que madeira não reflete muito)
        50,                        // shininess (superfície áspera, brilho baixo)
        texturaBola2
    );
    Material mat_bola3(
        Vector3d(0.2, 0.1, 0.05),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(1,1,1),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.3, 0.2, 0.1),   // ks (brilho baixo, já que madeira não reflete muito)
        50,                        // shininess (superfície áspera, brilho baixo)
        texturaBola3
    );
    Material mat_bola4(
        Vector3d(0.2, 0.1, 0.05),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(1,1,1),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.3, 0.2, 0.1),   // ks (brilho baixo, já que madeira não reflete muito)
        50,                        // shininess (superfície áspera, brilho baixo)
        texturaBola4
    );
    Material mat_bola5(
        Vector3d(0.2, 0.1, 0.05),  // ka (baixa reflexão ambiente, madeira absorve luz)
        Vector3d(1,1,1),   // kd (boa difusão da luz, textura amadeirada)
        Vector3d(0.3, 0.2, 0.1),   // ks (brilho baixo, já que madeira não reflete muito)
        50,                        // shininess (superfície áspera, brilho baixo)
        texturaBola5
    );
    double sphere_radius = 0.5;
    Vector3d sphere_center(0,-0.5,0);

    Vector3d plane_p0(0.0, -2.0, 0.0);
    Vector3d plane_normal(0.0, 1.0, 0.0);

    Vector3d plane2_p0( 0.0, 0.0, -6.0);
    Vector3d plane2_normal(0.0, 0.0, 1.0);
    Vector3d bg_color(0.0, 0.0, 0.0);

    Vector3d p0_plano_tras( 0.0, 0.0, 10.0);
    Vector3d normal_tras(0.0, 0.0, -1.0);
    Vector3d p0_plano_frente(0.0, 0.0, 0.0);
    Vector3d normal_frente(0.0, 0.0, 1.0);
    Vector3d p0_plano_esquerda(0.0, 0.0, 0.0);
    Vector3d normal_esquerda(1.0, 0.0, 0.0);
    Vector3d p0_plano_direita(10.0, 0.0, 0.0);
    Vector3d normal_direita(-1.0, 0.0, 0.0);
    Vector3d p0_plano_chao(0.0, 0.0, 0.0);
    Vector3d normal_chao(0.0, 1.0, 0.0);
    Vector3d p0_plano_teto(0.0, 3.0, 0.0);
    Vector3d normal_teto(0.0, -1.0, 0.0);

    Plano* plano_tras = new Plano(p0_plano_tras, normal_tras, mat_wall_madeira);
    Plano* plano_frente = new Plano(p0_plano_frente, normal_frente, mat_wall_madeira);
    Plano* plano_esquerda = new Plano(p0_plano_esquerda, normal_esquerda, mat_wall_madeira);
    Plano* plano_direita = new Plano(p0_plano_direita, normal_direita, mat_wall_madeira);
    Plano* plano_chao = new Plano(p0_plano_chao, normal_chao, mat_piso);
    Plano* plano_teto = new Plano(p0_plano_teto, normal_teto, mat_marmore);

    // Instanciando 5 esferas com raio 0.05
    Esfera* sphere5 = new Esfera(Vector3d(4.0, 1.05, 5.0), 0.05, mat_bola1);
    Esfera* sphere6 = new Esfera(Vector3d(6.0, 1.05, 5.05), 0.05, mat_bola2);
    Esfera* sphere7 = new Esfera(Vector3d(5.0, 1.05, 5.5), 0.05, mat_bola3);
    Esfera* sphere8 = new Esfera(Vector3d(5.5, 1.05, 4.5), 0.05, mat_bola4);
    Esfera* sphere9 = new Esfera(Vector3d(4.0, 1.05, 5.0), 0.05, mat_bola5);

    //MESA 
    std::vector<Eigen::Vector3d> vertices_mesa = {
        // Face superior
        Eigen::Vector3d(3.2, 1.0, 4.0),  // V0
        Eigen::Vector3d(6.2, 1.0, 4.0),  // V1
        Eigen::Vector3d(6.2, 1.0, 6.0),  // V2
        Eigen::Vector3d(3.2, 1.0, 6.0),  // V3
        
        // Face inferior (0.3 unidades abaixo)
        Eigen::Vector3d(3.2, 0.7, 4.0),  // V4
        Eigen::Vector3d(6.2, 0.7, 4.0),  // V5
        Eigen::Vector3d(6.2, 0.7, 6.0),  // V6
        Eigen::Vector3d(3.2, 0.7, 6.0)   // V7
    };
    std::vector<Triangulo> faces_mesa = {
        // Face superior
        Triangulo(vertices_mesa[0], vertices_mesa[1], vertices_mesa[2]),
        Triangulo(vertices_mesa[0], vertices_mesa[2], vertices_mesa[3]),
        
        // Face inferior
        Triangulo(vertices_mesa[4], vertices_mesa[5], vertices_mesa[6]),
        Triangulo(vertices_mesa[4], vertices_mesa[6], vertices_mesa[7]),
        
        // Laterais
        Triangulo(vertices_mesa[0], vertices_mesa[1], vertices_mesa[5]),
        Triangulo(vertices_mesa[0], vertices_mesa[5], vertices_mesa[4]),
        
        Triangulo(vertices_mesa[1], vertices_mesa[2], vertices_mesa[6]),
        Triangulo(vertices_mesa[1], vertices_mesa[6], vertices_mesa[5]),
        
        Triangulo(vertices_mesa[2], vertices_mesa[3], vertices_mesa[7]),
        Triangulo(vertices_mesa[2], vertices_mesa[7], vertices_mesa[6]),
        
        Triangulo(vertices_mesa[3], vertices_mesa[0], vertices_mesa[4]),
        Triangulo(vertices_mesa[3], vertices_mesa[4], vertices_mesa[7])
    };
    // Criar malha com material adequado (ex: madeira escura)
    Material mat_mesa_sinuca(
        Vector3d(0.0, 0.2, 0.0),   // ka - Reflexão ambiente levemente esverdeada
        Vector3d(0.0, 0.6, 0.0),   // kd - Difusa verde vibrante
        Vector3d(0.5, 0.5, 0.5),   // ks - Reflexão especular reduzida para um efeito mais fosco
        60                         // shininess - Reduzido para simular o feltro
    );
    

    Malha* mesa = new Malha(faces_mesa, vertices_mesa, mat_mesa_sinuca);



    Material formas = Material(
        Vector3d(0.7, 0.2, 0.2),
        Vector3d(0.7, 0.2, 0.2),
        Vector3d(0.7, 0.2, 0.2),
        10
    );


    // Adicao do cubo
    // Definição dos vértices do cubo (agora deslocado para a esquerda)
    std::vector<Eigen::Vector3d> vertices = {
        Eigen::Vector3d(-3,  1, -5), Eigen::Vector3d(-1,  1, -5),  // Superiores frontais
        Eigen::Vector3d(-1, -1, -5), Eigen::Vector3d(-3, -1, -5),  // Inferiores frontais
        Eigen::Vector3d(-3,  1, -7), Eigen::Vector3d(-1,  1, -7),  // Superiores traseiros
        Eigen::Vector3d(-1, -1, -7), Eigen::Vector3d(-3, -1, -7)   // Inferiores traseiros
    };

    // Definição das faces do cubo
    std::vector<Triangulo> faces = {
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

    Malha* malha = new Malha(faces, vertices, formas);
    // malha->translacao(Eigen::Vector3d(3, 3, 3));


    Esfera* sphere = new Esfera(sphere_center, sphere_radius, mat_metal);
    Esfera* sphere2 = new Esfera(Vector3d(1.5, 0.0, 0.0), 0.5, mat_plastico);
    Esfera* sphere3 = new Esfera(Vector3d(0.0, 0.0, 0.0), 0.5, mat_plastico);
    Esfera* sphere4 = new Esfera(Vector3d(-1.5, 0.0, 0.0), 0.5, mat_madeira);

    // Definir o material da lateral
Material mat_lateral(
    Vector3d(0.2, 0.1, 0.05),  // ka (reflexão ambiente)
    Vector3d(0.6, 0.4, 0.2),   // kd (difusão)
    Vector3d(0.3, 0.2, 0.1),   // ks (reflexão especular)
    10                         // shininess
);

// Coordenadas da mesa
double mesa_x_min = 3.2;
double mesa_x_max = 6.2;
double mesa_z_min = 4.0;
double mesa_z_max = 6.0;
double mesa_y = 1.0; // Altura da mesa

// Altura da lateral
double altura_lateral = 0.05;

// Vértices da lateral
std::vector<Eigen::Vector3d> vertices_lateral = {
    // Frente
    Eigen::Vector3d(mesa_x_min, mesa_y, mesa_z_min),                // V0
    Eigen::Vector3d(mesa_x_max, mesa_y, mesa_z_min),                // V1
    Eigen::Vector3d(mesa_x_min, mesa_y + altura_lateral, mesa_z_min), // V2
    Eigen::Vector3d(mesa_x_max, mesa_y + altura_lateral, mesa_z_min), // V3

    // Trás
    Eigen::Vector3d(mesa_x_min, mesa_y, mesa_z_max),                // V4
    Eigen::Vector3d(mesa_x_max, mesa_y, mesa_z_max),                // V5
    Eigen::Vector3d(mesa_x_min, mesa_y + altura_lateral, mesa_z_max), // V6
    Eigen::Vector3d(mesa_x_max, mesa_y + altura_lateral, mesa_z_max), // V7

    // Esquerda
    Eigen::Vector3d(mesa_x_min, mesa_y, mesa_z_min),                // V8 (V0)
    Eigen::Vector3d(mesa_x_min, mesa_y, mesa_z_max),                // V9 (V4)
    Eigen::Vector3d(mesa_x_min, mesa_y + altura_lateral, mesa_z_min), // V10 (V2)
    Eigen::Vector3d(mesa_x_min, mesa_y + altura_lateral, mesa_z_max), // V11 (V6)

    // Direita
    Eigen::Vector3d(mesa_x_max, mesa_y, mesa_z_min),                // V12 (V1)
    Eigen::Vector3d(mesa_x_max, mesa_y, mesa_z_max),                // V13 (V5)
    Eigen::Vector3d(mesa_x_max, mesa_y + altura_lateral, mesa_z_min), // V14 (V3)
    Eigen::Vector3d(mesa_x_max, mesa_y + altura_lateral, mesa_z_max)  // V15 (V7)
};

// Triângulos da lateral (cada retângulo é dividido em 2 triângulos)
std::vector<Triangulo> faces_lateral = {
    // Frente
    Triangulo(vertices_lateral[0], vertices_lateral[1], vertices_lateral[2]),
    Triangulo(vertices_lateral[1], vertices_lateral[3], vertices_lateral[2]),

    // Trás
    Triangulo(vertices_lateral[4], vertices_lateral[5], vertices_lateral[6]),
    Triangulo(vertices_lateral[5], vertices_lateral[7], vertices_lateral[6]),

    // Esquerda
    Triangulo(vertices_lateral[8], vertices_lateral[9], vertices_lateral[10]),
    Triangulo(vertices_lateral[9], vertices_lateral[11], vertices_lateral[10]),

    // Direita
    Triangulo(vertices_lateral[12], vertices_lateral[13], vertices_lateral[14]),
    Triangulo(vertices_lateral[13], vertices_lateral[15], vertices_lateral[14])
};

    // Criar a malha da lateral
    Malha* lateral_mesa = new Malha(faces_lateral, vertices_lateral, mat_lateral);
    // Distância entre as laterais
    double distancia_laterais = 0.01;
    double altura_lateral2 = 0.3;

    // Vértices da segunda lateral (deslocada 0.05 unidades para fora)
    std::vector<Eigen::Vector3d> vertices_lateral2 = {
        // Frente
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y, mesa_z_min - distancia_laterais),                // V0
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y, mesa_z_min - distancia_laterais),                // V1
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y + altura_lateral2, mesa_z_min - distancia_laterais), // V2
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y + altura_lateral2, mesa_z_min - distancia_laterais), // V3

        // Trás
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y, mesa_z_max + distancia_laterais),                // V4
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y, mesa_z_max + distancia_laterais),                // V5
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y + altura_lateral2, mesa_z_max + distancia_laterais), // V6
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y + altura_lateral2, mesa_z_max + distancia_laterais), // V7

        // Esquerda
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y, mesa_z_min - distancia_laterais),                // V8 (V0)
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y, mesa_z_max + distancia_laterais),                // V9 (V4)
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y + altura_lateral2, mesa_z_min - distancia_laterais), // V10 (V2)
        Eigen::Vector3d(mesa_x_min - distancia_laterais, mesa_y + altura_lateral2, mesa_z_max + distancia_laterais), // V11 (V6)

        // Direita
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y, mesa_z_min - distancia_laterais),                // V12 (V1)
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y, mesa_z_max + distancia_laterais),                // V13 (V5)
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y + altura_lateral2, mesa_z_min - distancia_laterais), // V14 (V3)
        Eigen::Vector3d(mesa_x_max + distancia_laterais, mesa_y + altura_lateral2, mesa_z_max + distancia_laterais)  // V15 (V7)
    };
    Material mat_lateral2(
        Vector3d(0.1, 0.05, 0.02),  // ka (reflexão ambiente)
        Vector3d(0.4, 0.2, 0.1),    // kd (difusão)
        Vector3d(0.2, 0.1, 0.05),   // ks (reflexão especular)
        10                          // shininess
    );
    
    // Triângulos da segunda lateral (cada retângulo é dividido em 2 triângulos)
    std::vector<Triangulo> faces_lateral2 = {
        // Frente
        Triangulo(vertices_lateral2[0], vertices_lateral2[1], vertices_lateral2[2]),
        Triangulo(vertices_lateral2[1], vertices_lateral2[3], vertices_lateral2[2]),

        // Trás
        Triangulo(vertices_lateral2[4], vertices_lateral2[5], vertices_lateral2[6]),
        Triangulo(vertices_lateral2[5], vertices_lateral2[7], vertices_lateral2[6]),

        // Esquerda
        Triangulo(vertices_lateral2[8], vertices_lateral2[9], vertices_lateral2[10]),
        Triangulo(vertices_lateral2[9], vertices_lateral2[11], vertices_lateral2[10]),

        // Direita
        Triangulo(vertices_lateral2[12], vertices_lateral2[13], vertices_lateral2[14]),
        Triangulo(vertices_lateral2[13], vertices_lateral2[15], vertices_lateral2[14])
    };

    // Criar a malha da segunda lateral
    Malha* lateral_mesa2 = new Malha(faces_lateral2, vertices_lateral2, mat_lateral);
    lateral_mesa2->translacao(Eigen::Vector3d(0, -0.3, 0));
    // Definir o material amarelo para o lustre
    Material mat_lustre(
        Vector3d(0.2, 0.2, 0.0),  // ka (reflexão ambiente)
        Vector3d(1.0, 1.0, 0.0),   // kd (difusão - cor amarela)
        Vector3d(0.5, 0.5, 0.0),   // ks (reflexão especular)
        50                         // shininess
    );
    
    // Parâmetros do cone (lustre)
    double raio_base_lustre = 0.5;
    double altura_lustre = 0.3;
    
    // Posição do cone (centro da base do cone)
    Vector3d posicao_lustre(4.7, 3.0 - altura_lustre, 5.0); // Centralizado acima da mesa
    
    // Direção do cone (apontando para baixo)
    Vector3d direcao_lustre(0.0, 1.0, 0.0);
    
    // Criar o cone (lustre)
    Cone* lustre = new Cone(altura_lustre, raio_base_lustre, direcao_lustre, posicao_lustre, mat_lustre);
    Material mat_taco(
        Vector3d(0.2, 0.1, 0.05),  // ka (reflexão ambiente)
        Vector3d(0.4, 0.2, 0.1),    // kd (difusão - cor marrom)
        Vector3d(0.3, 0.2, 0.1),    // ks (reflexão especular)
        20                          // shininess
    );
    
    // Parâmetros do taco
    double altura_taco = 2.0;       // Comprimento do taco
    double raio_taco = 0.02;        // Raio do taco
    double centro_y_taco = 1.07;     // Posição no eixo y
    
    // Posição do taco (centro do cilindro)
    Vector3d posicao_taco(4.7, centro_y_taco, 5.0); // Centralizado na mesa
    
    // Direção do taco (horizontal, ao longo do eixo x)
    Vector3d direcao_taco(-1.0, 0.0, 1.0);
    
    // Criar o cilindro (taco)
    Cilindro* taco = new Cilindro(altura_taco, raio_taco, direcao_taco, posicao_taco, mat_madeira);
    Plano* plane = new Plano(plane_p0, plane_normal, mat_tijolo);
    Plano* plane2 = new Plano(plane2_p0, plane2_normal, mat_marmore);
    
     
    Vector3d cylinder_base_center(1.375, 12.0, -(viewport_distance + 2.0 / 2.0));  // Posição do cilindro ao lado da esfera
    Vector3d cylinder_direction(1.0, 0.0, 0.0);
    Cilindro* cilindro = new Cilindro (2.5, 0.2, cylinder_direction, cylinder_base_center, mat_madeira);

    Vector3d cone_base_center(3.0, -2.0, -(viewport_distance + 2.0 / 2.0));
    Vector3d cone_direction(0.0, 1.0, 1.0);
    Cone* cone = new Cone (1.5, 1.0, cone_direction, cone_base_center, formas);


    Luz light1 = Luz(
        Vector3d(4.7,2.63,5),
        Vector3d(1.0, 1.0, 1.0),
        0.2
    );
/*     Luz light2 = Luz(
        Vector3d(0.0, 7000.0, 110000.0),
        Vector3d(1.0, 1.0, 1.0),
        0.6
    );
 */
    Vector3d ambient_light(1.0,1.0,1.0);

    Camera camera = Camera(p0, viewport_width, viewport_height, image_width, image_height, viewport_distance, bg_color);

    Vector3d camera_position = p0;
    double camera_speed = 1;

    Cena scene = Cena(ambient_light);

    scene.add_light(light1);
    scene.add_object(plano_tras);
    scene.add_object(plano_frente);
    scene.add_object(plano_esquerda);
    scene.add_object(plano_direita);
    scene.add_object(plano_chao);
    scene.add_object(plano_teto);
    // Adicionar à cena
    scene.add_object(mesa);
    Material mat_pes(
        Vector3d(0.1, 0.05, 0.02),  // ka - Reflexão ambiente com tom amadeirado
        Vector3d(0.5, 0.25, 0.1),   // kd - Difusa marrom quente
        Vector3d(0.2, 0.15, 0.1),   // ks - Reflexão especular reduzida para um brilho mais natural
        50                          // shininess - Ajustado para um brilho suave
    );
    scene.add_object(sphere5);
    scene.add_object(sphere6);
    scene.add_object(sphere7);
    scene.add_object(sphere8);
    scene.add_object(sphere9);
    scene.add_object(lateral_mesa);
    scene.add_object(lateral_mesa2);
    scene.add_object(lustre);
    scene.add_object(taco);
    // Lista de posições para os 4 pés (coordenadas dos cantos inferiores da mesa)
    vector<Vector3d> posicoes_pes = {
        Vector3d(3.2, 0.0, 4.0),   // Pé frontal esquerdo
        Vector3d(6.2 - 0.5, 0.0, 4.0),  // Pé frontal direito
        Vector3d(6.2 - 0.5, 0.0, 6.0 - 0.5),  // Pé traseiro direito
        Vector3d(3.2, 0.0, 6.0 - 0.5)   // Pé traseiro esquerdo
    };
    
    // Criar e configurar cada pé
    for(Vector3d pos : posicoes_pes) {
        // Criar cubo unitário
        vector<Vector3d> vertices_pe = {
            Vector3d(0,0,0), Vector3d(0.5,0,0),
            Vector3d(0.5,0,0.5), Vector3d(0,0,0.5),
            Vector3d(0,0.7,0), Vector3d(0.5,0.7,0),
            Vector3d(0.5,0.7,0.5), Vector3d(0,0.7,0.5)
        };
    
        vector<Triangulo> faces_pe = {
            // Face inferior
            Triangulo(vertices_pe[0], vertices_pe[1], vertices_pe[2]),
            Triangulo(vertices_pe[0], vertices_pe[2], vertices_pe[3]),
            // Face superior
            Triangulo(vertices_pe[4], vertices_pe[5], vertices_pe[6]),
            Triangulo(vertices_pe[4], vertices_pe[6], vertices_pe[7]),
            // Laterais
            Triangulo(vertices_pe[0], vertices_pe[1], vertices_pe[5]),
            Triangulo(vertices_pe[0], vertices_pe[5], vertices_pe[4]),
            Triangulo(vertices_pe[1], vertices_pe[2], vertices_pe[6]),
            Triangulo(vertices_pe[1], vertices_pe[6], vertices_pe[5]),
            Triangulo(vertices_pe[2], vertices_pe[3], vertices_pe[7]),
            Triangulo(vertices_pe[2], vertices_pe[7], vertices_pe[6]),
            Triangulo(vertices_pe[3], vertices_pe[0], vertices_pe[4]),
            Triangulo(vertices_pe[3], vertices_pe[4], vertices_pe[7])
        };
    
        Malha* pe = new Malha(faces_pe, vertices_pe, mat_pes);
        
        // Aplicar transformações
        pe->translacao(pos);  // Posicionar no canto correto
        
        scene.add_object(pe);
    }
    // contador de fps
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    // Setando o imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Criando variáveis booleanas para mostrar e desmostrar a interface de determinandas operacoes
    bool verGui = false;
    bool GuiTransformacao = false;
    bool GuiMaterial = false;
    bool GuiTranslacao = false;
    bool GuiEscalonamento = false;
    bool GuiRotQ = false;
    bool GuiRotE = false;
    bool GuiCisalhamento = false;
    bool GuiCena = false;
    bool addObjeto = false;
    bool addEsfera = false;
    bool addCilindro = false;
    bool addCone = false;
    bool addLuz = false;
    bool GuiCamera = false;

    Forma* selecionado = nullptr;

    // main loop
    SDL_Event event;
    //camera.draw_scene(renderer, scene);
    for (Forma* obj : scene.objects) {
        obj->setSelecionada(false);
    }
    while (true) {

        // Iniciar um novo frame do ImGui
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Gui principal de cada objeto para realizar operacoes com este
        if (verGui){
            // Criando a interface grafica
            ImGui::Begin("Modificações do objeto"); // Inicia uma nova janela
            ImGui::Text("O que você deseja fazer com este objeto?");  // Adiciona um texto
            if (ImGui::Button("Transformacoes")) { // Adiciona um botão
                verGui = false;
                GuiTransformacao = true;
            }

            if (ImGui::Button("Alterar material"))
            {
                verGui = false;
                GuiMaterial = true;
            }
            ImGui::End();
        }

        // Interface para alterar o material do objeto
        if (GuiMaterial)
        {

            ImGui::Begin("Mudar material");
            ImGui::Text("Selecione o material entre as opções");
            try
            {

            if (ImGui::Button("Madeira"))
            {
                if (selecionado != nullptr) { // Verifica se selecionado é válido
                    selecionado->setMaterial(mat_madeira);
                    cout << "Material alterado com sucesso!";
                    GuiMaterial = false;
                    verGui = true;
                    selecionado = nullptr;
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            if (ImGui::Button("Mármore"))
                {
                    if (selecionado != nullptr) { // Verifica se selecionado é válido
                        selecionado->setMaterial(mat_marmore);
                        cout << "Material alterado com sucesso!";
                        GuiMaterial = false;
                        verGui = true;
                        selecionado = nullptr;
                    } else {
                        std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                    }
                }

                if (ImGui::Button("Plástico"))
                {
                    if (selecionado != nullptr) { // Verifica se selecionado é válido
                        selecionado->setMaterial(mat_plastico);
                        cout << "Material alterado com sucesso!";
                        GuiMaterial = false;
                        verGui = true;
                        selecionado = nullptr;
                    } else {
                        std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                    }
                }

                if (ImGui::Button("Tijolo"))
                {
                    if (selecionado != nullptr) { // Verifica se selecionado é válido
                        selecionado->setMaterial(mat_tijolo);
                        cout << "Material alterado com sucesso!";
                        GuiMaterial = false;
                        verGui = true;
                        selecionado = nullptr;
                    } else {
                        std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                    }
                }

                if (ImGui::Button("Metal"))
                {
                    if (selecionado != nullptr) { // Verifica se selecionado é válido
                        selecionado->setMaterial(mat_metal);
                        cout << "Material alterado com sucesso!";
                        GuiMaterial = false;
                        verGui = true;
                        selecionado = nullptr;
                    } else {
                        std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                    }
                }

                if (ImGui::Button("Tijolo branco"))
                {
                    if (selecionado != nullptr) { // Verifica se selecionado é válido
                        selecionado->setMaterial(mat_tijolo_branco);
                        cout << "Material alterado com sucesso!";
                        GuiMaterial = false;
                        verGui = true;
                        selecionado = nullptr;
                    } else {
                        std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                    }
                }

                if (ImGui::Button("Madeira(Parede)"))
                {
                    if (selecionado != nullptr) { // Verifica se selecionado é válido
                        selecionado->setMaterial(mat_wall_madeira);
                        cout << "Material alterado com sucesso!";
                        GuiMaterial = false;
                        verGui = true;
                        selecionado = nullptr;
                    } else {
                        std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                    }
                }

            }

            catch (...)
            {
                cout << "Erro! Nao foi possivel aplicar este material ao objeto" << endl;
            }
            ImGui::End();

        }

        // GUI responsavel pelo gerenciamento da realizacao de transformacoes no objeto
        if (GuiTransformacao) {
            ImGui::Begin("Transformação do objeto");
            ImGui::Text("Selecione a transformação desejada");

            // Usando try e catch para tratar erros de transformacoes com objetos
            try {
                if (ImGui::Button("Translação")) {
                    GuiTranslacao = true;
                }

                if (ImGui::Button("Escalonamento"))
                {
                    GuiEscalonamento = true;
                }

                if (ImGui::Button("Cisalhamento"))
                {
                    GuiCisalhamento = true;
                }

                if (ImGui::Button("Rotação em torno de um dos eixos"))
                {
                    GuiRotE = true;
                }

                if (ImGui::Button("Rotação usando quatérnios"))
                {
                    GuiRotQ = true;
                }


            } catch (...) {
                std::cout << "Não foi possível realizar a transformação com os parâmetros e/ou o objeto selecionado" << std::endl;
            }

            ImGui::End();
        }

        // GUI para realizar a translacao do objeto
        if (GuiTranslacao)
        {

            static float x = 0;
            static float y = 0;
            static float z = 0;

            ImGui::Begin("Translação");
            ImGui::Text("Adicione a posição");

            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);

            if (ImGui::Button("Realizar translação"))
            {
                if (selecionado != nullptr) {
                    // Verifica o tipo do objeto selecionado e faz o downcasting
                    if (auto malha = dynamic_cast<Malha*>(selecionado)) {
                        malha->translacao(Eigen::Vector3d(x, y, z));
                    } else if (auto esfera = dynamic_cast<Esfera*>(selecionado)) {
                        esfera->translacao(Eigen::Vector3d(x, y, z));
                    } else if (auto cilindro = dynamic_cast<Cilindro*>(selecionado)) {
                        cilindro->translacao(Eigen::Vector3d(x, y, z));
                    } else if (auto cone = dynamic_cast<Cone*>(selecionado))
                    {
                        cone->translacao(Eigen::Vector3d(x, y, z));
                    }
                    else {
                        std::cerr << "Erro: Objeto não suporta translação!" << std::endl;
                    }
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            ImGui::End();

        }

        // GUI para realizar escalonamento nos objetos
        if (GuiEscalonamento)
        {

            static float x = 0;
            static float y = 0;
            static float z = 0;
            static float unico = 0;

            ImGui::Begin("Escalonamento");
            ImGui::Text("Adicione a posição");

            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);
            ImGui::InputFloat("Unico (Para objetos que não são malha)", &unico);

            if (ImGui::Button("Realizar escalonamento"))
            {
                if (selecionado != nullptr) {
                    // Verifica o tipo do objeto selecionado e faz o downcasting
                    if (auto malha = dynamic_cast<Malha*>(selecionado)) {
                        malha->escalonar(Eigen::Vector3d(x, y, z));
                    } else if (auto cone = dynamic_cast<Cone*>(selecionado)) {
                        cone->escalonar(unico);
                    } else if (auto cilindro = dynamic_cast<Cilindro*>(selecionado)) {
                        cilindro->escalonar(unico);
                    } else if (auto esfera = dynamic_cast<Esfera*>(selecionado)) {
                        esfera->escalonar(unico);
                    }
                    else {
                        std::cerr << "Erro: Objeto não suporta translação!" << std::endl;
                    }
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            ImGui::End();

        }

        // GUI para realizar rotacao em torno de algum eixo
        if (GuiRotE){

            static float angulo = 0;

            ImGui::Begin("Rotação em torno de um eixo");
            ImGui::InputFloat("Ângulo", &angulo);

            if (ImGui::Button("Em torno do eixo x"))
            {
                if (selecionado != nullptr) {
                    // Verifica o tipo do objeto selecionado e faz o downcasting
                    if (auto malha = dynamic_cast<Malha*>(selecionado)) {
                        malha->rotacionar_eixo('x', angulo);
                    } else if (auto cone = dynamic_cast<Cone*>(selecionado)) {
                        cone->rotacionar_eixo('x', angulo);
                    } else if (auto cilindro = dynamic_cast<Cilindro*>(selecionado)) {
                        cilindro->rotacionar_eixo('x', angulo);
                    } else {
                        std::cerr << "Erro: Objeto não suporta translação!" << std::endl;
                    }
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            if (ImGui::Button("Em torno do eixo y"))
            {
                if (selecionado != nullptr) {
                    // Verifica o tipo do objeto selecionado e faz o downcasting
                    if (auto malha = dynamic_cast<Malha*>(selecionado)) {
                        malha->rotacionar_eixo('y', angulo);
                    } else if (auto cone = dynamic_cast<Cone*>(selecionado)) {
                        cone->rotacionar_eixo('y', angulo);
                    } else if (auto cilindro = dynamic_cast<Cilindro*>(selecionado)) {
                        cilindro->rotacionar_eixo('y', angulo);
                    } else {
                        std::cerr << "Erro: Objeto não suporta translação!" << std::endl;
                    }
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            if (ImGui::Button("Em torno do eixo z"))
            {
                if (selecionado != nullptr) {
                    // Verifica o tipo do objeto selecionado e faz o downcasting
                    if (auto malha = dynamic_cast<Malha*>(selecionado)) {
                        malha->rotacionar_eixo('z', angulo);
                    } else if (auto cone = dynamic_cast<Cone*>(selecionado)) {
                        cone->rotacionar_eixo('z', angulo);
                    } else if (auto cilindro = dynamic_cast<Cilindro*>(selecionado)) {
                        cilindro->rotacionar_eixo('z', angulo);
                    } else {
                        std::cerr << "Erro: Objeto não suporta translação!" << std::endl;
                    }
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            ImGui::End();

        }

        // GUI para realizar rotacao usando quaternios
        if (GuiRotQ)
        {
            static float angulo = 0;
            static float x = 0;
            static float y = 0;
            static float z = 0;

            ImGui::Begin("Rotação com quatérnios");
            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);
            ImGui::InputFloat("Ângulo", &angulo);

            if (ImGui::Button("Rotacionar"))
            {
                if (selecionado != nullptr) {
                    // Verifica o tipo do objeto selecionado e faz o downcasting
                    if (auto malha = dynamic_cast<Malha*>(selecionado)) {
                        malha->rotacionar_quaternio(angulo, Eigen::Vector3d(x, y, z));
                    } else if (auto cone = dynamic_cast<Cone*>(selecionado)) {
                        cone->rotacionar_quaternio(angulo, Eigen::Vector3d(x, y, z));
                    } else if (auto cilindro = dynamic_cast<Cilindro*>(selecionado)) {
                        cilindro->rotacionar_quaternio(angulo, Eigen::Vector3d(x, y, z));
                    } else {
                        std::cerr << "Erro: Objeto não suporta translação!" << std::endl;
                    }
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            ImGui::End();

        }

        // GUI para realizar cisalhamento no objeto
        if (GuiCisalhamento)
        {
            static float xy = 0;
            static float xz = 0;
            static float yx = 0;
            static float yz = 0;
            static float zx = 0;
            static float zy = 0;

            ImGui::Begin("Cisalhamento");

            ImGui::InputFloat("XY", &xy);
            ImGui::InputFloat("XZ", &xz);
            ImGui::InputFloat("YX", &yx);
            ImGui::InputFloat("YZ", &yz);
            ImGui::InputFloat("ZX", &zx);
            ImGui::InputFloat("ZY", &zy);

            if (ImGui::Button("Cisalhar"))
            {
                if (selecionado != nullptr) {
                    // Verifica o tipo do objeto selecionado e faz o downcasting
                    if (auto malha = dynamic_cast<Malha*>(selecionado)) {
                        malha->cisalhar(1, 0, 0, 0, 0, 0);
                    } else {
                        std::cerr << "Erro: Objeto não suporta translação!" << std::endl;
                    }
                } else {
                    std::cerr << "Erro: Nenhum objeto selecionado!" << std::endl;
                }
            }

            ImGui::End();
        }

        if (GuiCena)
        {

            ImGui::Begin("Opções de cena");
            ImGui::Text("O que você gostaria de fazer?");
            if (ImGui::Button("Adicionar objeto"))
            {
                addObjeto = true;
            }

            if (ImGui::Button("Adicionar luz"))
            {
                addLuz = true;
            }

            if (ImGui::Button("Mudar a posição da câmera"))
            {
                GuiCamera = true;
            }

            ImGui::End();

        }

        if (addLuz)
        {

            static float intensidade = 0;

            static float x = 0;
            static float y = 0;
            static float z = 0;

            static float cor[3] = {0.0, 0.0, 0.0};

            ImGui::Begin("Adicionar luz");
            ImGui::Text("Adicione os fatores da luz");

            ImGui::InputFloat("Intensidade", &intensidade);

            ImGui::Text("Posição da luz");
            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);

            ImGui::Text("Defina a cor da luz");
            ImGui::ColorEdit3("Cor", cor);

            if (ImGui::Button("Adicionar luz"))
            {
                scene.add_light(Luz(Eigen::Vector3d(x, y, z), Eigen::Vector3d(cor[0], cor[1], cor[2]), intensidade));
                addLuz = false;
            }

            ImGui::End();

        }

        if (addObjeto)
        {
            ImGui::Begin("Adicionar objeto");
            ImGui::Text("Qual objeto você gostaria de adicionar?");
            if (ImGui::Button("Esfera"))
            {
                addEsfera = true;
                addObjeto = false;
            }

            if (ImGui::Button("Cilindro"))
            {
                addCilindro = true;
                addObjeto = false;
            }

            if (ImGui::Button("Cone"))
            {
                addCone = true;
                addObjeto = false;
            }

            ImGui::End();

        }

        if (addEsfera)
        {
            static float raio = 0;
            static float x = 0;
            static float y = 0;
            static float z = 0;

            ImGui::Begin("Adicionar Esfera");
            ImGui::Text("Adicione os fatores a seguir");
            ImGui::Text("Posição do centro da esfera");
            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);
            ImGui::Text("Raio da esfera");
            ImGui::InputFloat("Raio", &raio);

            if (ImGui::Button("Gerar esfera"))
            {
                scene.add_object(new Esfera(Eigen::Vector3d(x, y, z), raio, mat_default));
                addEsfera = false;
            }

            ImGui::End();

        }

        if (addCilindro)
        {
            static float raioBase = 0;
            static float altura = 0;

            static float dcX = 0;
            static float dcY = 0;
            static float dcZ = 0;

            static float x = 0;
            static float y = 0;
            static float z = 0;

            ImGui::Begin("Adicionar Cilindro");
            ImGui::Text("Adicione os fatores a seguir");

            ImGui::InputFloat("Raio da base", &raioBase);
            ImGui::InputFloat("Altura", &altura);

            ImGui::Text("Posição do centro da base");
            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);

            ImGui::Text("Direção do cilindro");
            ImGui::InputFloat("dcX", &dcX);
            ImGui::InputFloat("dcY", &dcY);
            ImGui::InputFloat("dcZ", &dcZ);

            if (ImGui::Button("Gerar cilindro"))
            {
                scene.add_object(new Cilindro(altura, raioBase, Eigen::Vector3d(dcX, dcY, dcZ), Eigen::Vector3d(x, y, z), mat_default));
                addCilindro = false;
            }

            ImGui::End();

        }

        if (addCone)
        {
            static float raioBase = 0;
            static float altura = 0;

            static float dcX = 0;
            static float dcY = 0;
            static float dcZ = 0;

            static float x = 0;
            static float y = 0;
            static float z = 0;

            ImGui::Begin("Adicionar Cone");
            ImGui::Text("Adicione os fatores a seguir");

            ImGui::InputFloat("Raio da base", &raioBase);
            ImGui::InputFloat("Altura", &altura);

            ImGui::Text("Posição do centro da base");
            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);

            ImGui::Text("Direção do cone");
            ImGui::InputFloat("dcX", &dcX);
            ImGui::InputFloat("dcY", &dcY);
            ImGui::InputFloat("dcZ", &dcZ);

            if (ImGui::Button("Gerar cone"))
            {
                scene.add_object(new Cone(altura, raioBase, Eigen::Vector3d(dcX, dcY, dcZ), Eigen::Vector3d(x, y, z), mat_default));
                addCone = false;
            }

            ImGui::End();

        }

        if (GuiCamera)
        {

            static float x = 0;
            static float y = 0;
            static float z = 0;

            ImGui::Begin("Posicionar câmera");
            ImGui::Text("Escreva a posição que você deseja colocar a câmera");

            ImGui::InputFloat("X", &x);
            ImGui::InputFloat("Y", &y);
            ImGui::InputFloat("Z", &z);

            if (ImGui::Button("Reposicionar câmera"))
            {
                camera_position[0] = x;
                camera_position[1] = y;
                camera_position[2] = z;
                camera.setPosition(camera_position);
                GuiCamera = false;
            }

            if (ImGui::Button("Onde estou?"))
            {
                cout << "x: " << camera_position[0] << " y: " << camera_position[1] << " z: " << camera_position[2] << endl;
            }

            ImGui::End();

        }

        // event handler
        while (SDL_PollEvent(&event) != 0) {

            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT) {
                goto quit;
            }

            else if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                verGui = false;
                GuiMaterial = false;
                GuiTransformacao = false;
                GuiTranslacao = false;
                GuiEscalonamento = false;
                GuiRotE = false;
                GuiRotQ = false;
                GuiCisalhamento = false;
                GuiCena = false;
                addObjeto = false;
                addLuz = false;
                addEsfera = false;
                addCilindro = false;
                addCone = false;
                GuiCamera = false;
            }

            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB)
            {
                GuiCena = true;

            }

            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: camera_position[2] -= 0.5; camera.setPosition(camera_position);break; // Frente
                    case SDLK_s: camera_position[2] += 0.5; camera.setPosition(camera_position);break; // Trás
                    case SDLK_a: camera_position[0] -= 0.5; camera.setPosition(camera_position);break; // Esquerda
                    case SDLK_d: camera_position[0] += 0.5; camera.setPosition(camera_position);break; // Direita
                    case SDLK_q: camera_position[1] += 0.5; camera.setPosition(camera_position);break; // Cima
                    case SDLK_e: camera_position[1] -= 0.5; camera.setPosition(camera_position);break; // Baixo

                    // Funcao para aplicar zoom in e zoom out, conforte e requisitado
                    case SDLK_o: camera.zoomIn(1.1); break; // zoom in
                    case SDLK_i: camera.zoomOut(1.1); break; // zoom out

                    case SDLK_f: malha->translacao(Eigen::Vector3d(1, 1, 1)); break;
                    case SDLK_g: malha->rotacionar_quaternio(45, Eigen::Vector3d(1, 1, 1)); break;
                    case SDLK_h: malha->rotacionar_eixo('x', 30); break;
                    case SDLK_j: malha->escalonar(Eigen::Vector3d(1.1, 1.1, 1.1)); break;
                    case SDLK_k: malha->cisalhar(0, 1, 0, 0, 0, 0); break;

                    //  rotação da câmera
                    case SDLK_m: // Rotacionar para a esquerda
                        camera.rotateYaw(5.0);
                        camera.draw_scene(renderer, scene);
                    break;
                    case SDLK_n: // Rotacionar para a direita
                        camera.rotateYaw(-5.0);
                        camera.draw_scene(renderer, scene);
                    break;
                    case SDLK_v: // Rotacionar para cima
                        camera.rotatePitch(5.0);
                        camera.draw_scene(renderer, scene);
                    break;
                    case SDLK_b: // Rotacionar para baixo
                        camera.rotatePitch(-5.0);
                        camera.draw_scene(renderer, scene);
                    break;

                    case SDLK_DELETE: {
                        // Procurar o objeto selecionado
                        selecionado = nullptr;
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

                            verGui = false;
                            GuiMaterial = false;
                            GuiTransformacao = false;
                        }
                        break;
                    }
                }

                //camera.setPosition(camera_position);



            }else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // verfica se o shift ou ctrl estão pressionados
                bool shiftPressed = (SDL_GetModState() & KMOD_SHIFT) != 0;
                bool ctrlPressed = (SDL_GetModState() & KMOD_CTRL) != 0;

                Eigen::Vector3d dr = ((camera.getViewport().p00 + camera.getViewport().dx * mouseX - camera.getViewport().dy * mouseY) - camera.pos).normalized();
                // Raio gerado a partir do clique do mouse (ponto de origem e direção)
                Raio raioCLique(camera.pos, dr);
                if(shiftPressed){
                    auto [forma_selecionada, t] = scene.get_closest_object(raioCLique);
                    Eigen::Vector3d pontoAlvo = raioCLique.Po + t.t * raioCLique.dr;
                    if (forma_selecionada) {
                        // Verifica se o objeto já está selecionado
                        if (forma_selecionada->isSelecioanda()) {
                            // Se já estiver selecionado, desmarque-o
                            forma_selecionada->setSelecionada(false);
                            selecionado = nullptr;
                            verGui = false;
                            GuiMaterial = false;
                            GuiTransformacao = false;
                        } else {
                            // Caso contrário, marque-o como selecionado
                            forma_selecionada->setSelecionada(true);
                            selecionado = forma_selecionada;
                            verGui = true;

                        }

                        // Re-renderizar cena
                        camera.draw_scene(renderer, scene);
                    }
                }
                if(ctrlPressed){
                    auto [forma_selecionada, t] = scene.get_closest_object(raioCLique);
                    Eigen::Vector3d pontoAlvo = raioCLique.Po + t.t * raioCLique.dr;
                    // Atualiza a câmera para olhar para o ponto clicado
                    camera.lookAt(pontoAlvo, Eigen::Vector3d(0, 1, 0));

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

    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
