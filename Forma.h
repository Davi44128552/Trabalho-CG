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
    virtual std::pair<double, double> getTextureCoords(const Eigen::Vector3d& point) const {
        return {0.0, 0.0}; 
    }
    virtual ~Forma() = default;
    void setSelecionada(bool estado){
        selecionada = estado;
    }
    bool isSelecioanda()const{
        return selecionada;
        bool selecionada;
    }

    void setMaterial(const Material& material)
    {
        this->material = material;
    }

protected:
    Material material;
    bool selecionada;
};

#endif // FORMA_H
