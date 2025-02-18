// Forma.h
#ifndef FORMA_H
#define FORMA_H

#include <Eigen/Dense>
#include "Raio.h"
#include "Material.h"

class Forma {
public:
    Forma(const Material& material) : material(material) {}
    virtual double obter_ti(const Raio& raio) const = 0;
    virtual Eigen::Vector3d obter_normal(const Eigen::Vector3d& ponto) const = 0;
    Material getMaterial() const {return material;}
    virtual ~Forma() = default;
    void setSelected(bool estado){
        selected = estado;
    }
    bool isSelected()const{
        return selected;
        bool selected;
    }
protected:
    Material material;
    bool selected;
};

#endif // FORMA_H
