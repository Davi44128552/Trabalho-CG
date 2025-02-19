# include "Plano.h"
#include "Intersecao.h"

// Implementando o construtor
Plano::Plano(Eigen::Vector3d ponto_plano, Eigen::Vector3d normal, const Material& material)
	: Forma(material), ponto_plano(ponto_plano), normal(normal.normalized()) {}

// Implementando a funcao que verifica se o raio intercepta o plano
Intersecao Plano::obter_intersecao(const Raio& raio) const{

	/* Quando realizamos a deducao da verificacao de interceptacao
	entre o raio e o plano, chegamos a seguinte formula: 
	ti = -(v*n) / dr*n onde v e Po-P_plano */
	Eigen::Vector3d v = raio.Po - ponto_plano;

	// Definindo um valor para verificar a precisao
	double epsilon = 0.000001;

	// Verificando se ha intersecao
	if (raio.dr.dot(normal) != 0){ // Verificando, inicialmente, se o denominador nao e zero
		double ti = -((v.dot(normal)) / (raio.dr.dot(normal)));

		// Verificando se ti e negativo
		if (ti >= epsilon){
			return Intersecao(true, ti, normal, raio.Po + raio.dr * ti);
		}
	}

	// Caso o ponto nao seja valido, entao retornamos nulo
	return Intersecao::NONE;

}
