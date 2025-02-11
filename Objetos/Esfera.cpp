#include "Esfera.hpp"

// Construtor
Esfera::Esfera(Eigen::Vector3d centroEsfera, double raioEsfera) {
    this->centroEsfera = centroEsfera;
    this->raioEsfera = raioEsfera;
}

// Método para calcular o ponto de interseção
double Esfera::obter_ti(const Raio& raio) const {
    // Obtendo o dr do raio
    Eigen::Vector3d dr = raio.dr;
    Eigen::Vector3d v = raio.Po - this->centroEsfera; // v = Po - C

    // Atribuindo os valores para a equação do segundo grau
    double a = dr.dot(dr);
    double b = 2 * (dr.dot(v));
    double c = v.dot(v) - (this->raioEsfera * this->raioEsfera);

    // Calculando o discriminante (delta)
    double discriminante = (b * b) - (4 * a * c);
    double ti;

    if (discriminante > 0) {
        // Pegando o menor ti válido
        ti = (-b - sqrt(discriminante)) / (2 * a);
        if (ti >= 0) {
            return ti;
        }

        ti = (-b + sqrt(discriminante)) / (2 * a);
        return ti;
    }

    if (discriminante == 0) {
        ti = (-b) / (2 * a);
        return ti;
    }

    return nan(""); // Caso não haja interseção, retorna NaN
}

// Funcao para calcular a normal da esfera
Eigen::Vector3d Esfera::obter_normal(Eigen::Vector3d pontoIntersecao){
		// A normal da esfera e dada pela normalizacao de P - C
        Eigen::Vector3d normal = pontoIntersecao - this->centroEsfera;
        normal = (normal) / (normal.norm());
        return normal;
    }
