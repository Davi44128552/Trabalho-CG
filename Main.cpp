#include <iostream>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include "Raio.h"
#include "Esfera.h"
#include "Iluminacao.h"
#include <algorithm>
#include "Plano.h"
#include "Cena.h"
#include "Cilindro.h" 
using namespace std;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "Falha ao iniciar SDL: " << SDL_GetError() << endl;
        return -1;
    }

    int nColunas = 400;
    int nLinhas = 400;

    SDL_Window* window = SDL_CreateWindow("Canvas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nColunas, nLinhas, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Falha ao criar a janela: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Falha ao criar o renderizador: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    double height = 2;
    double width = 2;
    double dJanela = 3;
    Eigen::Vector3d posicao_observador = Eigen::Vector3d::Zero();

    Eigen::Vector3d ponto_superior_esquerdo(-width / 2, height / 2, -dJanela);
    double delta_x = width / nColunas;
    double delta_y = height / nLinhas;

    Eigen::Vector3d posicao_luz(0, 3, 0);
    Iluminacao iluminacao(
        Eigen::Vector3d(0.5, 0.5, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4),
        Eigen::Vector3d(0.7, 0.7, 0.7), Eigen::Vector3d(0.6, 0.6, 0.6),
        Eigen::Vector3d(0.7, 0.7, 0.7), Eigen::Vector3d(0.2, 0.2, 0.2), 1.5);

    Cena cena(posicao_luz, iluminacao);

    bool running = true;
    SDL_Event event;

    double velocidade = 0.1;  // Velocidade do movimento da câmera

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: posicao_observador[2] -= velocidade; break; // Frente
                    case SDLK_s: posicao_observador[2] += velocidade; break; // Trás
                    case SDLK_a: posicao_observador[0] -= velocidade; break; // Esquerda
                    case SDLK_d: posicao_observador[0] += velocidade; break; // Direita
                    case SDLK_q: posicao_observador[1] += velocidade; break; // Cima
                    case SDLK_e: posicao_observador[1] -= velocidade; break; // Baixo
                }
            }
        }

        // Limpa a tela antes de re-renderizar
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Renderiza a cena com a nova posição do observador
        cena.renderizar(renderer, nColunas, nLinhas, posicao_observador, ponto_superior_esquerdo, delta_x, delta_y, dJanela);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
