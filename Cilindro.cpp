# include "Cilindro.h"
#include "Eigen/src/Core/Matrix.h"
#include "Intersecao.h"
#include <cmath>
#include <Eigen/Geometry>

// Implementando o construtor do cilindro
Cilindro::Cilindro(double altura, double raio_base, Eigen::Vector3d dc, Eigen::Vector3d centro_base, const Material& material)
	: Forma(material), altura(altura), raio_base(raio_base), dc(dc.normalized()), centro_base(centro_base) {}

// Implementando a funcao para verificar se existe intersecao entre o raio e o cilindro
Intersecao Cilindro::obter_intersecao(const Raio& raio) const {

	// Criando variaveis que vao assumir valores para o calculo da discriminante
    /* Quando calculamos a deducao do cilindro, chegamos a
	i*i + 2*j*i + j*j, onde 
	i = dr - (dr*dc) * dc
	j = Po - centro_base - ((Po-centro_base)*dc) * dc) */
    Eigen::Vector3d i = raio.dr - (dc * (raio.dr.dot(dc)));
    Eigen::Vector3d j = (raio.Po - centro_base) - (dc * (dc.dot(raio.Po - centro_base)));
    
    // Coeficientes da equacao quadratica
    double a = i.dot(i);
    double b = 2.0 * j.dot(i);
    double c = j.dot(j) - (raio_base * raio_base);
    
    // Verificando o valor do discriminante
    double discriminante = b*b - 4.0 * a * c;
    if (discriminante < 0.0) {
        return Intersecao::NONE;
    }
    
    // Calculando os valores para t
    double t1 = (-b -std::sqrt(discriminante)) / (2.0 * a);
	double t2 = (-b +std::sqrt(discriminante)) / (2.0 * a);
    double menor_t_valido = INFINITY; // menor t da colisão do raio com a superficie cilindrica
    double altura_menor_t; // altura do menor t
    Eigen::Vector3d normal_menor_t;
    Eigen::Vector3d pi_menor_t; // ponto de interseção do menor t

	// Verificando se os valores sao validos para os pontos
	for (double t: {t1, t2}){

		Eigen::Vector3d ponto_intersecao = raio.Po + t*raio.dr; // Ponto de intersecao no cilindro

        double altura_t = (ponto_intersecao - centro_base).dot(dc);
		bool validade_ponto = t > 0.0; // Verificando se t > 0
		bool validade_base = altura_t >= 0.0;
		bool validade_altura = altura_t < this->altura;
        bool validade_menor_t = t < menor_t_valido;

		// Verificando se todas as condicoes sao respeitadas
		if (validade_ponto && validade_base && validade_altura && validade_menor_t) {
			menor_t_valido = t;
            altura_menor_t = altura_t;
            pi_menor_t = ponto_intersecao;
            Eigen::Vector3d proj = centro_base + (dc * altura_menor_t);
            normal_menor_t = (pi_menor_t - proj).normalized();
		}

	}

    // Check top plane of the cylinder
    double bottom_topo = raio.dr.dot(dc);
    if (bottom_topo != 0.0) {
        double t_tampa = -(raio.Po - (centro_base + dc*altura)).dot(dc) / bottom_topo;

        if (t_tampa >= 0.0 && t_tampa < menor_t_valido &&
            ((raio.Po + t_tampa*raio.dr) - (centro_base + dc*altura)).norm() <= raio_base) {
            menor_t_valido = t_tampa;
            normal_menor_t = dc;
        }
    }

    // Check base plane of the cylinder
    double bottom_base = raio.dr.dot(-dc);
    if (bottom_base != 0.0) {
        double t_base = -(raio.Po - centro_base).dot(-dc) / bottom_base;

        if (t_base >= 0.0 && t_base < menor_t_valido &&
            ((raio.Po + t_base*raio.dr) - centro_base).norm() <= raio_base) {
            menor_t_valido = t_base;
            normal_menor_t = -dc;
        }
    }

    if (menor_t_valido < INFINITY) {
        return Intersecao(true, menor_t_valido, normal_menor_t, pi_menor_t);
    }
    
    return Intersecao::NONE;
}
std::pair<double, double> Cilindro::getTextureCoords(const Eigen::Vector3d& point) const {
    // Vetor do centro da base até o ponto de interseção
    Eigen::Vector3d to_center = point - centro_base;
    
    // Altura ao longo do eixo do cilindro
    double h = to_center.dot(dc);
    
    // Componente radial (ortogonal ao eixo dc)
    Eigen::Vector3d radial_vec = to_center - h * dc;

    // Caso o ponto esteja exatamente no eixo (radial_vec muito pequeno)
    if (radial_vec.squaredNorm() < 1e-9) {
        return {0.5, h / altura}; // Centro da textura em u
    }

    // Encontra vetores ortogonais no plano perpendicular a dc
    Eigen::Vector3d u = dc.unitOrthogonal(); // Vetor perpendicular a dc
    Eigen::Vector3d v = dc.cross(u).normalized();

    // Calcula o ângulo theta em torno do eixo
    double theta = atan2(radial_vec.dot(v), radial_vec.dot(u));

    // Mapeia theta para [0, 1] e a altura para v
    double u_coord = (theta + M_PI) / (2 * M_PI);
    double v_coord = h / altura;

    return {u_coord, v_coord};
}

// Funcao para calcular o centroide
Eigen::Vector3d Cilindro::calcularCentroide() const {
    Eigen::Vector3d centro_topo = centro_base + dc * altura; // Calculando o ponto do centro do topo do cilindro
    Eigen::Vector3d centroide = (centro_base + centro_topo) / 2.0; // Calculo do centroide
    return centroide;
}

void Cilindro::translacao(Eigen::Vector3d d){
  centro_base = centro_base + d;
}

void Cilindro::escalonar(double d){
  altura = altura * d;
  raio_base = raio_base * d;
}

void Cilindro::rotacionar_eixo(char eixo, double angulo){

  // Obtendo o centroide e realizando translacao
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
void Cilindro::rotacionar_quaternio(double angulo, Eigen::Vector3d eixo) {
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