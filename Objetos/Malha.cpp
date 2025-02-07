#include "Malha.h"
#include <limits>
#include <cmath>

// Construtor
Malha::Malha(std::vector<Triangulo> faces, std::vector<Vector3d> vertices) {
    this->id_triangulo_atual = 0;
    this->faces = faces;
    this->vertices = vertices;
}

// Funcao para obter o ponto de intersecao do raio com algum triangulo da malha
double Malha::obter_ti(const Raio& raio) {
    double menor_ti = std::numeric_limits<double>::infinity(); // Inicializa com infinito
    bool encontrou_intersecao = false;

    for (Triangulo& face : faces) {
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
Vector3d Malha::obter_normal() {
    return normal;
}
