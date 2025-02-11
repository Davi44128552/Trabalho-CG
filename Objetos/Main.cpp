#include <iostream>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include "Raio.hpp"
#include "Esfera.hpp"
#include "Plano.hpp"
#include "Iluminacao.hpp"
#include "Cilindro.hpp"
#include "Cone.hpp"
#include "Triangulo.hpp"
#include "Malha.hpp"

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
    Eigen::Vector3d posicao_observador(0.0, 0.0, 0.0);

    Eigen::Vector3d ponto_superior_esquerdo(-width / 2, height / 2, -dJanela);
    double delta_x = width / nColunas;
    double delta_y = height / nLinhas;

    Eigen::Vector3d posicao_luz(1.0, 1.0, -dJanela);
    Iluminacao iluminacao(
        Eigen::Vector3d(0.5, 0.5, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4),
        Eigen::Vector3d(0.7, 0.7, 0.7), Eigen::Vector3d(0.6, 0.6, 0.6),
        Eigen::Vector3d(0.7, 0.7, 0.7), Eigen::Vector3d(0.2, 0.2, 0.2), 1.5);

    /*std::vector<Eigen::Vector3d> vertices = {
        Eigen::Vector3d(0, 1, -5),  // Topo
        Eigen::Vector3d(-1, 0, -4.5), Eigen::Vector3d(1, 0, -4.5),
        Eigen::Vector3d(1, 0, -5.5), Eigen::Vector3d(-1, 0, -5.5),
        Eigen::Vector3d(0, -1, -5)  // Base
    };

    std::vector<Triangulo> faces = {
        // Metade superior
        Triangulo(vertices[0], vertices[1], vertices[2]),
        Triangulo(vertices[0], vertices[2], vertices[3]),
        Triangulo(vertices[0], vertices[3], vertices[4]),
        Triangulo(vertices[0], vertices[4], vertices[1]),
        
        // Metade inferior
        Triangulo(vertices[5], vertices[1], vertices[2]),
        Triangulo(vertices[5], vertices[2], vertices[3]),
        Triangulo(vertices[5], vertices[3], vertices[4]),
        Triangulo(vertices[5], vertices[4], vertices[1])
    };

    Malha malha(faces, vertices); */
    Triangulo triangulo(Vector3d(-0.5, 0.0, -dJanela), Vector3d(0.5, 0.0, -dJanela), Vector3d(0.0, 0.5, -dJanela));
    //triangulo.escalonar(Vector3d(2.0, 2.0, 2.0));
    //triangulo.cisalhar(0.5, 0.0, 0.0, 0.0, 0.0, 0.0);


    double z = -dJanela;
    for (int linhas = 0; linhas < nLinhas; linhas++) {
        double y = ponto_superior_esquerdo[1] - delta_y / 2 - delta_y * linhas;
        for (int colunas = 0; colunas < nColunas; colunas++) {
            double x = ponto_superior_esquerdo[0] + delta_x / 2 + delta_x * colunas;

            Eigen::Vector3d dr(x, y, -dJanela);
            Raio raio(posicao_observador, dr);

            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawPoint(renderer, colunas, linhas);

            double ponto = triangulo.obter_ti(raio);
            if (!isnan(ponto)) {
                Eigen::Vector3d ponto_intersecao(x * ponto, y * ponto, z * ponto);
                Eigen::Vector3d visao = -dr;
                Eigen::Vector3d luz = (posicao_luz - ponto_intersecao).normalized();
                Eigen::Vector3d normal = triangulo.obter_normal();

                Eigen::Vector3d cor(80, 255, 80);
                Eigen::Vector3d iluminacao_total = iluminacao.calcular_iluminacao_Total(luz, normal, visao).cwiseProduct(cor);

                Uint8 r = static_cast<Uint8>(std::clamp(iluminacao_total[0], 0.0, 255.0));
                Uint8 g = static_cast<Uint8>(std::clamp(iluminacao_total[1], 0.0, 255.0));
                Uint8 b = static_cast<Uint8>(std::clamp(iluminacao_total[2], 0.0, 255.0));

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, colunas, linhas);
            }
        }
    }

    SDL_RenderPresent(renderer);

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
