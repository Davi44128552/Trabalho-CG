#ifndef INTERSECAO_H
#define INTERSECAO_H

#include <Eigen/Dense>
#include <cmath>
#include "Eigen/src/Core/Matrix.h"
#include "Raio.h"
#include "Material.h"

class Intersecao {
public:
    static const Intersecao NONE;

    double t;
    Eigen::Vector3d normal;
    Eigen::Vector3d ponto_intersecao;
    bool existe;

    // Construtor
    Intersecao(bool existe, double t, Eigen::Vector3d normal, Eigen::Vector3d ponto_intersecao);
};

#endif // INTERSECAO_H
