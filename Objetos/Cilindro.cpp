# include "Cilindro.h"

// Implementando o construtor do cilindro
Cilindro::Cilindro(double altura, double raio_base, Eigen::Vector3d dc, Eigen::Vector3d centro_base){

	this->altura = altura;
	this->raio_base = raio_base;
	this->dc = dc.normalized();
	this->centro_base = centro_base;
	this->posicao = 'i';

}

// Implementando a funcao para verificar se existe intersecao entre o raio e o cilindro
double Cilindro::obter_ti(const Raio& raio){

	/* Quando calculamos a deducao do cilindro, chegamos a
	i*i + 2*j*i + j*j, onde 
	i = dr - (dr*dc) * dc
	j = Po - centro_base - ((Po-centro_base)*dc) * dc) */

	// Projecoes dos vetores no espaco perpendicular ao eixo do cilindro
	Eigen::Vector3d i = raio.dr - (raio.dr.dot(dc) * dc);
	Eigen::Vector3d j = (raio.Po - centro_base) - ((raio.Po - centro_base).dot(dc)*dc);

	// Coeficientes da equacao quadratica
	double a = i.dot(i);
	double b = 2 * j.dot(i);
	double c = j.dot(j) - (raio_base * raio_base);

	// Adicionando uma tolerancia para evitar problemas de precisao
	double epsilon = 0.000001;

	// Caso a < epsilon, entao o vetor de direcao e paralelo ao eixo do cilindro
	if (abs(a) < epsilon){
		return nan(""); // Retornando nulo para caso seja paralelo
	}

	// Calculando, por fim, o discriminante
	double discriminante = b*b - 4 * a * c;

	// Verificando o valor do discriminante
	if (discriminante < 0){
		return nan("");
	}

	// Calculando os valores para t
	double t1 = (-b -std::sqrt(discriminante)) / (2.0 * a);
	double t2 = (-b +std::sqrt(discriminante)) / (2.0 * a);

	// Verificando se os valores sao validos
	for (double t: {t1, t2}){

		if (t > 0){

			// Verificando se 0 <= (P-centro_base)*dc <= H
			Eigen::Vector3d ponto_intersecao = raio.Po + t*raio.dr;
			if (0 <= (ponto_intersecao - centro_base).dot(dc) && (ponto_intersecao - centro_base).dot(dc) <= altura){

				// Definindo em que parte estamos do cilindro, caso estejamos no cilindro
				if ((ponto_intersecao - centro_base).dot(dc) == 0){ // Neste caso, estaremos na base do cilindro
					posicao = 'b'; // base
				}

				else if ((ponto_intersecao - centro_base).dot(dc) == altura){ // Neste caso, estaremos no topo do cilindro
					posicao = 't'; // topo
				}

				else{ // Por fim, neste caso, estaremos no meio do cilindro, se estivermos no cilindro
					posicao = 'm'; // meio
				}

				return t;
			}

		}

	}

	return nan("");

}

// Implementando uma funcao para obter a normal do objeto
Eigen::Vector3d Cilindro::obter_normal(Eigen::Vector3d pontoIntersecao){

	// Caso estejamos no topo do cilindro, a nossa normal vai ser o dc
	if (posicao == 't'){
		return dc;
	}

	// Caso estejamos na base do cilindro, a nossa normal vai ser -dc
	else if (posicao == 'b'){
		return -dc;
	}

	// Por fim, caso nao estejamos no topo ou na base, quer dizer entao que estamos na superficie lateral do cilindro
	Eigen::Vector3d proj = pontoIntersecao - (pontoIntersecao.dot(dc) / dc.squaredNorm()) * dc;
    return proj.normalized();

}
