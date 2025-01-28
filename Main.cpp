#include <iostream>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include "Raio.h"
#include "Esfera.h"
#include "Iluminacao.h"
#include <algorithm>
#include "Plano.h"
#include "Cena.h"
using namespace std;

int main() {
    // Inicializando o SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "Falha ao iniciar SDL: " << SDL_GetError() << endl;
        return -1;
    }

    // Dimensões da tela
    int nColunas = 1000;
    int nLinhas = 1000;

    // Criando a janela
    SDL_Window* window = SDL_CreateWindow("Canvas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nColunas, nLinhas, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Falha ao criar a janela: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    // Criando o renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Falha ao criar o renderizador: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Variáveis da tela/canvas
    double height = 2;
    double width = 2;
    double dJanela = 3;
    Eigen::Vector3d posicao_observador = Eigen::Vector3d::Zero();

    Eigen::Vector3d ponto_superior_esquerdo(-width / 2, height / 2, -dJanela);
    double delta_x = width / nColunas;
    double delta_y = height / nLinhas;

    // Definindo a posicao da fonte luminosa
    Eigen::Vector3d posicao_luz(0, 5, 0);
    Iluminacao iluminacao(
        Eigen::Vector3d(0.5, 0.5, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4),
        Eigen::Vector3d(0.7, 0.7, 0.7), Eigen::Vector3d(0.6, 0.6, 0.6),
        Eigen::Vector3d(0.7, 0.7, 0.7), Eigen::Vector3d(0.2, 0.2, 0.2), 1.5);

     Cena cena(posicao_luz, iluminacao);
    

    // Renderizar a cena
    cena.renderizar(renderer, nColunas, nLinhas, posicao_observador, ponto_superior_esquerdo, delta_x, delta_y, dJanela);

    // Loop da janela
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}