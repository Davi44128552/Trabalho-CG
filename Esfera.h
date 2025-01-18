#ifndef ESFERA_H
#define ESFERA_H

#include <Eigen/Dense>
#include "Raio.h"
#include "Forma.h"

class Esfera : public Forma {
public:
    // Construtor
    Esfera(Eigen::Vector3d centroEsfera, double raioEsfera);
    // Método para obter o ponto de interseção
    double obter_ti(const Raio& raio) const override;
    // Método para obter a normal da esfera
    Eigen::Vector3d obter_normal(const Eigen::Vector3d& ponto) const override;

private:
    // Atributos da esfera
    Eigen::Vector3d centroEsfera;
    double raioEsfera;
};

#endif // ESFERA_H
