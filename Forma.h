// Forma.h
#ifndef FORMA_H
#define FORMA_H

#include <Eigen/Dense>
#include "Raio.h"
#include "Material.h"
#include "Intersecao.h"

class Forma {
public:
    Forma(const Material& material) : material(material) {}
    virtual Intersecao obter_intersecao(const Raio& raio) const = 0;
    Material getMaterial() const {return material;}
    virtual ~Forma() = default;
protected:
    Material material;
};

#endif // FORMA_H
