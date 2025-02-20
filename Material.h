// Material.h
#ifndef MATERIAL_H
#define MATERIAL_H

#include <Eigen/Dense>

class Material {
public:
    Material(const Eigen::Vector3d& ka, const Eigen::Vector3d& kd, const Eigen::Vector3d& ks, double shininess)
        : ka(ka), kd(kd), ks(ks), shininess(shininess) {}

    Eigen::Vector3d getKa() const { return ka; }
    Eigen::Vector3d getKd() const { return kd; }
    Eigen::Vector3d getKs() const { return ks; }
    double getShininess() const { return shininess; }

private:
    Eigen::Vector3d ka; // Coeficiente de reflexão ambiente
    Eigen::Vector3d kd; // Coeficiente de reflexão difusa
    Eigen::Vector3d ks; // Coeficiente de reflexão especular
    double shininess;   // Expoente de brilho
};

#endif // MATERIAL_H