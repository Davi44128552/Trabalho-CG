// Material.h
#ifndef MATERIAL_H
#define MATERIAL_H

#include <Eigen/Dense>
#include "Texture.h"

class Material {
public:
    Material(const Eigen::Vector3d& ka, const Eigen::Vector3d& kd, const Eigen::Vector3d& ks, double shininess)
        : ka(ka), kd(kd), ks(ks), shininess(shininess), texture(nullptr) {}
    Material(const Eigen::Vector3d& ka, const Eigen::Vector3d& kd, const Eigen::Vector3d& ks, double shininess, Texture* texture)
        : ka(ka), kd(kd), ks(ks), shininess(shininess), texture(texture) {}
    Eigen::Vector3d getKa() const { return ka; }
    Eigen::Vector3d getKd() const { return kd; }
    Eigen::Vector3d getKs() const { return ks; }
    double getShininess() const { return shininess; }
    // Verifica se o material usa uma textura
    bool hasTexture() const { return texture != nullptr; }

    // Obtém a cor da textura nas coordenadas (u, v)
    Eigen::Vector3d getTextureColor(double u, double v) const {
        if (texture) {
            return texture->sample(u, v);
        }
        return Eigen::Vector3d(1.0, 1.0, 1.0); // Retorna branco se não houver textura
    }

private:
    Eigen::Vector3d ka; // Coeficiente de reflexão ambiente
    Eigen::Vector3d kd; // Coeficiente de reflexão difusa
    Eigen::Vector3d ks; // Coeficiente de reflexão especular
    double shininess;   // Expoente de brilho
    Texture* texture;   // Textura associada ao material
};

#endif // MATERIAL_H