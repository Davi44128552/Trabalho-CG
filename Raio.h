#ifndef RAIO_H
#define RAIO_H

#include <Eigen/Dense>

class Raio {
public:
    // Atributos
    Eigen::Vector3d P;
    Eigen::Vector3d Po;
    Eigen::Vector3d dr;
    
    // Construtores
    Raio(Eigen::Vector3d Po, Eigen::Vector3d dr);

    // Métodos para acessar a origem e a direção do raio
    Eigen::Vector3d obter_origem() const { return Po; }
    Eigen::Vector3d obter_direcao() const { return dr; }
};

#endif // RAIO_H
