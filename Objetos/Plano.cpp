# include "Plano.h"

// Implementando o construtor
Plano::Plano(Eigen::Vector3d ponto_plano, Eigen::Vector3d normal){
	this->ponto_plano = ponto_plano;
	this->normal = normal;
}

// Implementando a funcao que verifica se o raio intercepta o plano
double Plano::obter_ti(const Raio& raio) const {

	/* Quando realizamos a deducao da verificacao de interceptacao
	entre o raio e o plano, chegamos a seguinte formula: 
	ti = -(v*n) / dr*n onde v e Po-P_plano */
	Eigen::Vector3d v = raio.Po - ponto_plano;

	// Verificando se ha intersecao
	if (raio.dr.dot(normal) != 0){ // Verificando, inicialmente, se o denominador nao e zero
		double ti = -((v.dot(normal)) / (raio.dr.dot(normal)));

		// Verificando se ti e negativo
		if (ti >= 0){
			return ti;
		}
	}

	// Caso o ponto nao seja valido, entao retornamos nulo
	return nan(""); // Caso não haja interseção, retorna NaN

}

// Implementando a funcao para obter a normal
// Neste caso, so temos que fazer um simples getter
Eigen::Vector3d Plano::obter_normal(){
	return normal;
}
