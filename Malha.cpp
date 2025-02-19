#include "Malha.h"
#include <cmath>
#include "Eigen/src/Core/Matrix.h"
#include "Intersecao.h"

// Construtor
Malha::Malha(std::vector<Triangulo> faces, std::vector<Eigen::Vector3d> vertices, const Material& material)
	: Forma(material), faces(faces), vertices(vertices) {}

// Funcao para obter o ponto de intersecao do raio com algum triangulo da malha
Intersecao Malha::obter_intersecao(const Raio& raio) const {
    double menor_ti = INFINITY; // Inicializa com infinito
    bool encontrou_intersecao = false;
    Eigen::Vector3d normal;
    Eigen::Vector3d ponto_intersecao;

    for (const Triangulo& face : faces) {
        double ti = face.obter_ti(raio);
        
        if (!std::isnan(ti) && ti < menor_ti) {
            menor_ti = ti;
            normal = face.obter_normal();
            encontrou_intersecao = true;
        }
    }
    
    ponto_intersecao = raio.Po + menor_ti * raio.dr;
    return Intersecao(encontrou_intersecao, menor_ti, normal, ponto_intersecao);
}
