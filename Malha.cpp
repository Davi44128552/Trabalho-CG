#include "Malha.h"
#include <limits>
#include <cmath>

// Construtor
Malha::Malha(std::vector<Triangulo> faces, std::vector<Eigen::Vector3d> vertices, const Material& material)
	: Forma(material), faces(faces), vertices(vertices) {}

// Funcao para obter o ponto de intersecao do raio com algum triangulo da malha
double Malha::obter_ti(const Raio& raio) const {
    double menor_ti = std::numeric_limits<double>::infinity(); // Inicializa com infinito
    bool encontrou_intersecao = false;

    for (const Triangulo& face : faces) {
        double ti = face.obter_ti(raio);
        
        if (!std::isnan(ti) && ti < menor_ti) {
            menor_ti = ti;
            normal = face.obter_normal(); // Atualiza a normal com a do triângulo mais próximo
            encontrou_intersecao = true;
        }
    }

    return encontrou_intersecao ? menor_ti : nan("");
}

// Funcao para calcular a normal da malha
Eigen::Vector3d Malha::obter_normal(const Eigen::Vector3d& ponto) const {
    return normal;
}
