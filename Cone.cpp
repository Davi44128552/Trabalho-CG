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
    double a = raio.dr.dot(dc) * raio.dr.dot(dc) - raio.dr.dot(raio.dr) * cos2_theta;
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

// Implementando funcao para calcular o centroide de um cone
Eigen::Vector3d Cone::calcularCentroide() const {
    Eigen::Vector3d vertice = centro_base + dc * altura; // Calculando o vertice do triangulo ("centro topo" do cone)
    Eigen::Vector3d centroide = centro_base + (vertice - centro_base) * (1.0 / 4.0); // Calculando o centroide
    return centroide;
}

// Implementando funcao de translacao
void Cone::translacao(Eigen::Vector3d d) {
  centro_base = centro_base + d;
}

// Implementando a funcao de escalonamento
void Cone::escalonar(double d) {
  altura = altura * d;
  raio_base = raio_base * d;
}

// Implementando a funcao para rotacionar em torno de um eixo(x, y ou z)
void Cone::rotacionar_eixo(char eixo, double angulo) {

  // Calculando o centroide e realizando translacao
  Eigen::Vector3d centroide = calcularCentroide();
  translacao(-centroide);

  // Definindo a matriz
  Eigen::Matrix3d K;

  // Convertendo o angulo para radianos
  double anguloRad = angulo * M_PI / 180.0;

  // Verificando em torno de qual eixo sera rotacionado
  if (eixo == 'x'){
    K <<    1, 0, 0,
            0, cos(anguloRad), -sin(anguloRad),
            0, sin(anguloRad), cos(anguloRad);
  }

  else if (eixo == 'y'){
    K <<    cos(anguloRad), 0, sin(anguloRad),
            0, 1, 0,
            -sin(anguloRad), 0, cos(anguloRad);
  }

  else if (eixo == 'z'){
    K <<    cos(anguloRad), -sin(anguloRad), 0,
            sin(anguloRad), cos(anguloRad), 0,
            0, 0, 1;
  }

  else{
    return; // Para casos invalidos
  }

  // Reposicionando pontos e a diracao do cilindro de acordo com as matrizes
  dc = K * dc;
  centro_base = K * centro_base;

  translacao(centroide);

}

// Implementando funcao de rotacao usando quaternios
void Cone::rotacionar_quaternio(double angulo, Eigen::Vector3d eixo) {
  	// Realizando a translacao com o centroide
    Eigen::Vector3d centroide = calcularCentroide();
    translacao(-centroide);

	// Convertendo o angulo de graus para radianos
	double anguloRad = angulo * M_PI / 180.0;

	// Normalizando o eixo de rotacao
	eixo.normalize();

	// Criando o quatérnio de rotação
	Eigen::Quaterniond q;
	q = Eigen::AngleAxisd(anguloRad, eixo);

	// Aplicando a rotacao ao centro da base e a dc
    centro_base = q * centro_base;
    dc = q * dc;

    // Realizando outra translacao para trazer o cilindro de volta ao seu lugar
    translacao(centroide);
}
