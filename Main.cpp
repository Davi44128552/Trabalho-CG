# include <iostream>
# include <Eigen/Dense>
# include <SDL2/SDL.h>
# include "Raio.h"
# include "Esfera.h"

using namespace std;

int main() {
    // Inicializando o SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "Falha ao iniciar SDL: " << SDL_GetError() << endl;
        return -1;
    }

    // Dimensões da tela
    int nColunas = 800;
    int nLinhas = 800;

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

    // Criando os objetos
        // Esfera
    Esfera esfera(Eigen::Vector3d(0.0, 0.0, -(dJanela + 0.4)), 0.4);

    // Pintando o canvas
    double z = -dJanela;
    for (int linhas = 0; linhas < nLinhas; linhas++) {
        // Definindo a posicao y
        double y = ponto_superior_esquerdo[1] - delta_y / 2 - delta_y * linhas;

        for (int colunas = 0; colunas < nColunas; colunas++) {
            // Definindo a posicao x
            double x = ponto_superior_esquerdo[0] + delta_x / 2 + delta_x * colunas;
            Eigen::Vector3d dr(x, y, -dJanela);

            // Criando um raio
            Raio raio(posicao_observador, dr);

            // Teste de SDL
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawPoint(renderer, colunas, linhas);

            // Verificando a intersecao com os objetos
                // Esfera
            double ponto = esfera.obter_ti(raio);
            if (!isnan(ponto)){ // Verificando se houve intersecao com a esfera

                Eigen::Vector3d ponto_intersecao(x*ponto, y*ponto, z*ponto); // Atualizando o ponto para o real ponto no espaco tridimensional
                SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
                SDL_RenderDrawPoint(renderer, colunas, linhas);

            }
        }
    }

    SDL_RenderPresent(renderer);

    // Loop principal do SDL
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    // Limpeza
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
