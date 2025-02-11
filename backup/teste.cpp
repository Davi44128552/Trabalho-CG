#include <iostream>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include "Raio.h"
#include "Esfera.h"
#include <algorithm>
using namespace std;

int ddmain() {
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

    // Criando os objetos
    Esfera esfera(Eigen::Vector3d(0.0, 0.0, -(dJanela + 0.4)), 0.4);
    Plano plano(Eigen::Vector3d(0.0, -0.5, 0.0), Eigen::Vector3d(0.0, 1.0, 0.0));

    // Pintando o canvas

    // Definindo as dimensoes (x, y, z)
    double z = -dJanela;
    for (int linhas = 0; linhas < nLinhas; linhas++) {
        double y = ponto_superior_esquerdo[1] - delta_y / 2 - delta_y * linhas;

        for (int colunas = 0; colunas < nColunas; colunas++) {
            double x = ponto_superior_esquerdo[0] + delta_x / 2 + delta_x * colunas;

            // Definindo o vetor dr e o raio para verificar as colisoes
            Eigen::Vector3d dr(x, y, -dJanela);
            Raio raio(posicao_observador, dr);

            // Pintando a tela inteira de cinza para depois verificar a colisao dos objetos
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawPoint(renderer, colunas, linhas);

            // Verificando se ocorre intersecao entre o raio e o objeto ESFERA
            double ponto_esfera = esfera.obter_ti(raio);
            double ponto_plano = plano.obter_ti(raio);

            if (!isnan(ponto_esfera) || !isnan(ponto_plano)) {
                Eigen::Vector3d ponto_intersecao;
                Eigen::Vector3d normal;
                Eigen::Vector3d cor;

                if (!isnan(ponto_esfera) && (isnan(ponto_plano) || ponto_esfera < ponto_plano)) {
                    ponto_intersecao = Eigen::Vector3d(x * ponto_esfera, y * ponto_esfera, z * ponto_esfera);
                    normal = esfera.obter_normal(ponto_intersecao).normalized();
                    cor = Eigen::Vector3d(255, 80, 80);
                } else {
                    ponto_intersecao = Eigen::Vector3d(x * ponto_plano, y * ponto_plano, z * ponto_plano);
                    normal = plano.obter_normal(ponto_intersecao).normalized();
                    cor = Eigen::Vector3d(80, 80, 255);
                }

                // Calculando os vetores necessarios para analisar a iluminacao total do objeto
                Eigen::Vector3d visao = -dr;
                Eigen::Vector3d luz = (posicao_luz - ponto_intersecao).normalized();

                // Verificando se há sombra
                Raio raio_sombra(ponto_intersecao, luz);
                double sombra_esfera = esfera.obter_ti(raio_sombra);
                double sombra_plano = plano.obter_ti(raio_sombra);

                if (!isnan(sombra_esfera) || !isnan(sombra_plano)) {
                    cor *= 0.2; // Aplicando sombra
                } else {
                    Eigen::Vector3d iluminacao_total = iluminacao.calcular_iluminacao_Total(luz, normal, visao).cwiseProduct(cor);
                    cor = iluminacao_total;
                }

                // Convertendo os valores totais da aplicacao da iluminacao ao espectro rgb
                Uint8 r = static_cast<Uint8>(std::clamp(cor[0], 0.0, 255.0));
                Uint8 g = static_cast<Uint8>(std::clamp(cor[1], 0.0, 255.0));
                Uint8 b = static_cast<Uint8>(std::clamp(cor[2], 0.0, 255.0));

                // Pintando o objeto com a sua cor definida pela iluminacao
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, colunas, linhas);
            }
        }
    }

    SDL_RenderPresent(renderer);

    // Loop da janela para ficar rodando
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
