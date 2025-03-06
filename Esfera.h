#ifndef ESFERA_H
#define ESFERA_H

#include <Eigen/Dense>
#include "Raio.h"
#include "Forma.h"
#include "Material.h"

class Esfera : public Forma {
public:
    // Construtor
    Esfera(Eigen::Vector3d centroEsfera, double raioEsfera, const Material& material);
    // Método para obter o ponto de interseção
    Intersecao obter_intersecao(const Raio& raio) const override;

    // Funcoes para transformacoes (cabiveis a este objeto)
    void translacao(Eigen::Vector3d d);
    void escalonar(double d);

private:
    // Atributos da esfera
    Eigen::Vector3d centroEsfera;
    double raioEsfera;
};

#endif // ESFERA_H
