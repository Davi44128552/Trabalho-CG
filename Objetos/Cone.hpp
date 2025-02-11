# ifndef CONE_H
# define CONE_H

# include <Eigen/Dense>
# include <iostream>
# include <cmath>
# include "Raio.hpp"

// Classe do cone
class Cone {

private:
	// Atributos do cone
	Eigen::Vector3d vertice; // Posicao do vertice do cone
	Eigen::Vector3d dc; // Vetor da direcao do cone
	double altura; // Altura do cone
	double raio_base; // Raio da base do cone
	char posicao; // Valor que vai definir se estamos na base ou no meio do cone

public:
	// Construtor
	Cone(Eigen::Vector3d vertice, Eigen::Vector3d dc, double altura, double raio_base);

	// Funcao para verificar a intersecao do raio com o objeto
	double obter_ti(const Raio& raio);

	// Funcao para obter a normal do objeto
	Eigen::Vector3d obter_normal(Eigen::Vector3d ponto_intersecao);

};

# endif