#include "Iluminacao.h"
#include <algorithm> // Para std::max

using namespace std;

// Construtor
Iluminacao::Iluminacao(const Eigen::Vector3d& intensidade_Ambiente, const Eigen::Vector3d& k_Ambiente, 
                       const Eigen::Vector3d& intensidade_Difusa, const Eigen::Vector3d& k_Difusa, 
                       const Eigen::Vector3d& intensidade_Especular, const Eigen::Vector3d& k_Especular, double shininess) {
    this->intensidade_Ambiente = intensidade_Ambiente;
    this->k_Ambiente = k_Ambiente;
    this->intensidade_Difusa = intensidade_Difusa;
    this->k_Difusa = k_Difusa;
    this->intensidade_Especular = intensidade_Especular;
    this->k_Especular = k_Especular;
    this->shininess = shininess;
}

// Ambiente
void Iluminacao::calcular_iluminacao_Ambiente() {
    iluminacao_Ambiente = k_Ambiente.cwiseProduct(intensidade_Ambiente);
}

// Difusa
void Iluminacao::calcular_iluminacao_Difusa(const Eigen::Vector3d& luz, const Eigen::Vector3d& normal) {
    iluminacao_Difusa = (k_Difusa.cwiseProduct(intensidade_Difusa)) * std::max(0.0, luz.dot(normal));
}

// Especular
void Iluminacao::calcular_iluminacao_Especular(const Eigen::Vector3d& luz, const Eigen::Vector3d& normal, const Eigen::Vector3d& visao) {
    Eigen::Vector3d r = 2 * (luz.dot(normal)) * normal - luz;
    iluminacao_Especular = (k_Especular.cwiseProduct(intensidade_Especular)) * pow(std::max(0.0, r.dot(visao)), shininess);
}

// Total
Eigen::Vector3d Iluminacao::calcular_iluminacao_Total(const Eigen::Vector3d& luz, const Eigen::Vector3d& normal, const Eigen::Vector3d& visao) {
    calcular_iluminacao_Ambiente();
    calcular_iluminacao_Difusa(luz, normal);
    calcular_iluminacao_Especular(luz, normal, visao);

    iluminacao_Total = iluminacao_Ambiente + iluminacao_Difusa + iluminacao_Especular;
    iluminacao_Total = iluminacao_Total.cwiseMax(0).cwiseMin(1);
    return iluminacao_Total;
}

Eigen::Vector3d Iluminacao::retornar_iluminacao_Ambiente() {
    return k_Ambiente.cwiseProduct(intensidade_Ambiente);
}