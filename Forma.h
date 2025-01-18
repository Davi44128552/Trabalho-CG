// Forma.h
#ifndef FORMA_H
#define FORMA_H

#include <Eigen/Dense>
#include "Raio.h"

class Forma {
public:
    virtual double obter_ti(const Raio& raio) const = 0;
    virtual Eigen::Vector3d obter_normal(const Eigen::Vector3d& ponto) const = 0;
    virtual ~Forma() = default;
};

#endif // FORMA_H
