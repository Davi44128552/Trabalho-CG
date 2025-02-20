#include "Cone.h"
#include "Eigen/src/Core/Matrix.h"
#include "Intersecao.h"
#include <cmath>

// Implementando o construtor do cone
Cone::Cone(double altura, double raio_base, Eigen::Vector3d dc, Eigen::Vector3d centro_base, const Material& material)
    : Forma(material), altura(altura), raio_base(raio_base), dc(dc.normalized()), centro_base(centro_base) {}

// Implementando a função para verificar se existe interseção entre o raio e o cone
Intersecao Cone::obter_intersecao(const Raio& raio) const {
    // Vetor do vértice do cone ao ponto de origem do raio
    Eigen::Vector3d co = raio.Po - (centro_base + dc * altura);

    // Coeficientes da equação quadrática para o cone
    double cos_theta = altura / std::sqrt(altura * altura + raio_base * raio_base);
    double cos2_theta = cos_theta * cos_theta;
    double a = raio.dr.dot(dc) * raio.dr.dot(dc) - cos2_theta;
    double b = 2.0 * (raio.dr.dot(dc) * co.dot(dc) - raio.dr.dot(co) * cos2_theta);
    double c = co.dot(dc) * co.dot(dc) - co.dot(co) * cos2_theta;

    // Verificando o valor do discriminante
    double discriminante = b * b - 4.0 * a * c;
    if (discriminante < 0.0) {
        return Intersecao::NONE;
    }

    // Calculando os valores para t
    double t1 = (-b - std::sqrt(discriminante)) / (2.0 * a);
    double t2 = (-b + std::sqrt(discriminante)) / (2.0 * a);
    double menor_t_valido = INFINITY; // menor t da colisão do raio com a superfície cônica
    double altura_menor_t; // altura do menor t
    Eigen::Vector3d normal_menor_t;
    Eigen::Vector3d pi_menor_t; // ponto de interseção do menor t

    // Verificando se os valores são válidos para os pontos
    for (double t : {t1, t2}) {
        Eigen::Vector3d ponto_intersecao = raio.Po + t * raio.dr; // Ponto de interseção no cone

        double altura_t = (ponto_intersecao - centro_base).dot(dc);
        bool validade_ponto = t > 0.0; // Verificando se t > 0
        bool validade_base = altura_t >= 0.0;
        bool validade_altura = altura_t < this->altura;
        bool validade_menor_t = t < menor_t_valido;

        // Verificando se todas as condições são respeitadas
        if (validade_ponto && validade_base && validade_altura && validade_menor_t) {
            menor_t_valido = t;
            altura_menor_t = altura_t;
            pi_menor_t = ponto_intersecao;
            Eigen::Vector3d proj = centro_base + (dc * altura_menor_t);
            normal_menor_t = (pi_menor_t - proj).normalized();
        }
    }

    // Check base plane of the cone
    double bottom_base = raio.dr.dot(-dc);
    if (bottom_base != 0.0) {
        double t_base = -(raio.Po - centro_base).dot(-dc) / bottom_base;

        if (t_base >= 0.0 && t_base < menor_t_valido &&
            ((raio.Po + t_base * raio.dr) - centro_base).norm() <= raio_base) {
            menor_t_valido = t_base;
            normal_menor_t = -dc;
        }
    }

    if (menor_t_valido < INFINITY) {
        return Intersecao(true, menor_t_valido, normal_menor_t, pi_menor_t);
    }

    return Intersecao::NONE;
}