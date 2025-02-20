#include "Luz.h"

Luz::Luz(Eigen::Vector3d posicao, Eigen::Vector3d cor, double intensidade) {
    this->posicao = posicao;
    this->cor = cor;
    this->intensidade = intensidade;
}
