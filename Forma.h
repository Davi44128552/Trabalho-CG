// Forma.h
#ifndef FORMA_H
#define FORMA_H

#include <Eigen/Dense>
#include "Raio.h"
#include "Material.h"

class Forma {
public:
    Forma(const Eigen::Vector3d& cor, const Material& material) : cor(cor), material(material) {}
    virtual double obter_ti(const Raio& raio) const = 0;
    virtual Eigen::Vector3d obter_normal(const Eigen::Vector3d& ponto) const = 0;
    Eigen::Vector3d getCor() const { return cor; }
    Material getMaterial() const {return material;}
    virtual ~Forma() = default;
protected:
    Material material;
    Eigen::Vector3d cor;
};

#endif // FORMA_H
