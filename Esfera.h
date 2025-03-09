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
    std::pair<double, double> getTextureCoords(const Eigen::Vector3d& point) const override {
        Eigen::Vector3d dir = (point - centroEsfera).normalized();
        double u = 0.5 + atan2(dir.z(), dir.x()) / (2 * M_PI);
        double v = 0.5 - asin(dir.y()) / M_PI;
        return {u, v};
    }

    // Funcoes para transformacoes (cabiveis a este objeto)
    void translacao(Eigen::Vector3d d);
    void escalonar(double d);

private:
    // Atributos da esfera
    Eigen::Vector3d centroEsfera;
    double raioEsfera;
};

#endif // ESFERA_H
