# include "Plano.h"
#include "Intersecao.h"
#include "cmath"
// Implementando o construtor
Plano::Plano(Eigen::Vector3d ponto_plano, Eigen::Vector3d normal, const Material& material)
	: Forma(material), ponto_plano(ponto_plano), normal(normal.normalized()) {
		Eigen::Vector3d v(0.0, 1.0, 0.0);
		tangente = this->normal.cross(v);
		if(tangente.squaredNorm() < 1e-6){
			v = Eigen::Vector3d(0.0, 0.0, 1.0);
			tangente = this->normal.cross(v);
		}
		tangente.normalize();
		bitangente = this->normal.cross(tangente).normalized();
	}

std::pair<double, double> Plano::getTextureCoords(const Eigen::Vector3d& point) const {
	// Vetor do ponto de referência do plano até o ponto de interseção
	Eigen::Vector3d vec = point - ponto_plano;
	
	// Projetar nos vetores tangente e bitangente
	double u = vec.dot(tangente);
	double v = vec.dot(bitangente);
	
	// Aplicar repetição da textura (wrap around)
	u = std::fmod(u, 1.0);
	v = std::fmod(v, 1.0);
	
	// Garantir valores positivos
	if (u < 0.0) u += 1.0;
	if (v < 0.0) v += 1.0;
	
	return {u, v};
}
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
