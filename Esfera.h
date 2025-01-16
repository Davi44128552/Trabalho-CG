#ifndef ESFERA_H
#define ESFERA_H

#include <Eigen/Dense>
#include <cmath>
#include "Raio.h"

class Esfera {
private:
    // Atributos da esfera
    Eigen::Vector3d centroEsfera;
    double raioEsfera;

public:
    // Construtor
    Esfera(Eigen::Vector3d centroEsfera, double raioEsfera);

    // Metodo para calcular o ponto de intersecao
    double obter_ti(const Raio& raio) const;

    // Metodo para obter a normal da esfera
    Eigen::Vector3d obter_normal(Eigen::Vector3d pontoIntersecao);
};

#endif // ESFERA_H
