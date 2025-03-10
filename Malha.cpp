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

    Eigen::Vector3d Malha::calcular_normal_media() const {
    Eigen::Vector3d normal_media = Eigen::Vector3d::Zero();

    // Soma as normais de todas as faces
    for (const Triangulo& face : faces) {
        normal_media += face.obter_normal();
    }

    // Normaliza o resultado
    normal_media.normalize();
    return normal_media;
}
    
    ponto_intersecao = raio.Po + menor_ti * raio.dr;
    return Intersecao(encontrou_intersecao, menor_ti, normal_media, ponto_intersecao);
}

// Implementando a função de translacao
void Malha::translacao(Eigen::Vector3d d){
  for (Triangulo& face : faces) {
    face.translacao(d);
  }
}

void Malha::rotacionar_quaternio(double angulo, Eigen::Vector3d eixo){
  Eigen::Vector3d centroide = calcularCentroide();
  translacao(-centroide);
  for (Triangulo& face : faces) {
    face.rotacionar_quaternio(angulo, eixo);
  }
  translacao(centroide);
}

void Malha::rotacionar_eixo(char eixo, double angulo){
  Eigen::Vector3d centroide = calcularCentroide();
  translacao(-centroide);
  for (Triangulo& face : faces) {
    face.rotacionar_eixo(eixo, angulo);
  }
  translacao(centroide);
}

void Malha::escalonar(Eigen::Vector3d s){

  	Eigen::Vector3d centroide = calcularCentroide();
    translacao(-centroide);
    for (Triangulo& face : faces) {
      face.escalonar(s);
    }
    translacao(centroide);

}

void Malha::cisalhar(double shXY, double shXZ, double shYX, double shYZ, double shZX, double shZY){
  Eigen::Vector3d centroide = calcularCentroide();
  translacao(-centroide);
  for (Triangulo& face : faces) {
    face.cisalhar(shXY, shXZ, shYX, shYZ, shZX, shZY);
  }
  translacao(centroide);
}

Eigen::Vector3d Malha::calcularCentroide() const {
    Eigen::Vector3d centroide_total(0, 0, 0);
    double area_total = 0.0;

    for (const Triangulo& face : faces) {
        // Calcula o centroide do triângulo
        Eigen::Vector3d centroide_face = (face.vertice1 + face.vertice2 + face.vertice3) / 3.0;

        // Calcula a área do triângulo usando produto vetorial
        Eigen::Vector3d v0 = face.vertice2 - face.vertice1;
        Eigen::Vector3d v1 = face.vertice3 - face.vertice1;
        double area = 0.5 * v0.cross(v1).norm();

        // Soma os produtos dos centroides pela área
        centroide_total += centroide_face * area;
        area_total += area;
    }

    // Retorna o centroide ponderado pela área total
    return (area_total > 0) ? centroide_total / area_total : Eigen::Vector3d(0, 0, 0);
}
