#ifndef LUZ_H
#define LUZ_H
#include <Eigen/Dense>

class Luz {
public:
    Eigen::Vector3d posicao;
    Eigen::Vector3d cor;
    double intensidade;

    Luz(Eigen::Vector3d posicao, Eigen::Vector3d cor, double intensidade);
};

#endif // LUZ_H
