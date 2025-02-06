#ifndef CENA_H
#define CENA_H

#include <Eigen/Dense>
#include <vector>
#include "Raio.h"
#include "Esfera.h"
#include "Forma.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "Material.h"

class Cena {
public:
    // Construtor
    Cena(const Eigen::Vector3d& posicao_luz);
    
    // Método para calcular e renderizar a cena
    void renderizar(SDL_Renderer* renderer, int nColunas, int nLinhas, const Eigen::Vector3d& posicao_observador, 
                    const Eigen::Vector3d& ponto_superior_esquerdo, double delta_x, double delta_y, double z);

    // Métodos auxiliares para lidar com colisões e iluminação
private:
    Eigen::Vector3d posicao_luz;
    std::vector<Forma*> objetos; // Lista de formas da cena

    // Verifica a interseção e calcula a iluminação
    void calcularIluminacao(Raio& raio, SDL_Renderer* renderer, int colunas, int linhas, double x, double y);
};

#endif // CENA_H
