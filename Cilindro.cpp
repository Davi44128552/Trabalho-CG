# include "Cilindro.h"

// Implementando o construtor do cilindro
Cilindro::Cilindro(double altura, double raio_base, Eigen::Vector3d dc, Eigen::Vector3d centro_base, const Material& material)
	: Forma(material), altura(altura), raio_base(raio_base), dc(dc.normalized()), centro_base(centro_base) {}

// Implementando a funcao para verificar se existe intersecao entre o raio e o cilindro
double Cilindro::obter_ti(const Raio& raio)const{

	// Criando variaveis que vao assumir valores para o calculo da discriminante
    /* Quando calculamos a deducao do cilindro, chegamos a
	i*i + 2*j*i + j*j, onde 
	i = dr - (dr*dc) * dc
	j = Po - centro_base - ((Po-centro_base)*dc) * dc) */
    Eigen::Vector3d i = raio.dr - (dc * (raio.dr.dot(dc)));
    Eigen::Vector3d j = (raio.Po - centro_base) - (dc * (dc.dot(raio.Po - centro_base)));
    
    // Coeficientes da equacao quadratica
    double a = i.dot(i);
    double b = 2 * j.dot(i);
    double c = j.dot(j) - (raio_base * raio_base);
    
    // Verificando o valor do discriminante
    double discriminante = b*b - 4 * a * c;
    if (discriminante < 0) {
        return nan(""); // Não há interseção
    }
    
    // Calculando os valores para t
    double t1 = (-b -std::sqrt(discriminante)) / (2.0 * a);
	double t2 = (-b +std::sqrt(discriminante)) / (2.0 * a);

	// Verificando se os valores sao validos para os pontos
	for (double t: {t1, t2}){

		Eigen::Vector3d ponto_intersecao = raio.Po + t*raio.dr; // Ponto de intersecao no cilindro

		bool validade_ponto = t > 0; // Verificando se t > 0
		bool validade_base = (ponto_intersecao - centro_base).dot(dc) >= 0;
		bool validade_altura = (ponto_intersecao - centro_base).dot(dc) < altura;

		// Verificando se todas as condicoes sao respeitadas
		if (validade_ponto && validade_base && validade_altura){
			return t;
		}

	}
    
    return nan(""); // Nenhuma interseção válida
}

// Método para obter a normal do cilindro
Eigen::Vector3d Cilindro::obter_normal(const Eigen::Vector3d& pontoIntersecao) const{

	// Calculando a nossa posicao exata no cone, para saber qual deve ser a sua normal
    double posicao_ponto = dc.dot(pontoIntersecao - centro_base);

    // Adicionando uma tolerancia para evitar problemas de precisao
    double epsilon = 0.000001;
    
    // Verificando se estamos na base do cilindro
    if (posicao_ponto < epsilon) {
        return -dc; // Normal apontando para baixo
    }
    
    // Verificando se estamos no topo do cilindro
    if (std::abs(posicao_ponto - altura) < epsilon) {
        return dc; // Normal apontando para cima
    }
    
    // Caso nao estejamos nem na base e nem no topo, entao estamos no meio
    Eigen::Vector3d proj = centro_base + (dc * posicao_ponto);
    return (pontoIntersecao - proj).normalized();
}
