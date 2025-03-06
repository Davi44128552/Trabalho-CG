#include "Esfera.h"
#include "Eigen/src/Core/Matrix.h"
#include "Intersecao.h"

// Construtor
Esfera::Esfera(Eigen::Vector3d centroEsfera, double raioEsfera, const Material& material)
    : Forma(material), centroEsfera(centroEsfera), raioEsfera(raioEsfera) {}

// Método para calcular o ponto de interseção
Intersecao Esfera::obter_intersecao(const Raio& raio) const{
    // Obtendo o dr do raio
    Eigen::Vector3d dr = raio.dr;
    Eigen::Vector3d v = raio.Po - this->centroEsfera; // v = Po - C

    // Atribuindo os valores para a equação do segundo grau
    double a = dr.dot(dr);
    double b = 2 * (dr.dot(v));
    double c = v.dot(v) - (this->raioEsfera * this->raioEsfera);

    // Calculando o discriminante (delta)
    double discriminante = (b * b) - (4 * a * c);

    if (discriminante < 0) {
	    return Intersecao::NONE;
    }

    // Pegando o menor ti válido
    double ti1 = (-b - sqrt(discriminante)) / (2 * a);
    double ti2 = (-b + sqrt(discriminante)) / (2 * a);
    double ti = ti2 < 0.0 || ti1 < ti2 ? ti1 : ti2;

    if (ti >= 0) {
	    Eigen::Vector3d ponto_intersecao = raio.Po + raio.dr * ti;
        Eigen::Vector3d normal = ponto_intersecao - this->centroEsfera;
        normal = normal.normalized();
        return Intersecao(true, ti, normal, ponto_intersecao);
    } else {
        return Intersecao::NONE;
    }

}

// Implementando a funcao para realizar translacao
void Esfera::translacao(Eigen::Vector3d d){
  centroEsfera = centroEsfera + d;
}

// Implementando a funcao para "escalonar"
void Esfera::escalonar(double d){
    raioEsfera = raioEsfera * d;
}