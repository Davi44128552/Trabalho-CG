# ifndef CILINDRO_H
# define CILINDRO_H

# include <Eigen/Dense>
# include <cmath>
# include "Raio.h"

// Classe do cilindro
class Cilindro{

private:
	// Atributos do cilindro
	double altura; // Altura do cilindro
	double raio_base; // Tamanho do raio da base
	Eigen::Vector3d dc; // Vetor unitario que indica a direcao do cilindro
	Eigen::Vector3d centro_base; // Posicao no espaco em que se encontra o centro da base da esfera
	char posicao; // Valor que vai definir se estamos no topo, base ou no meio do cilindro

public:
	// Construtor do objeto
	Cilindro(double altura, double raio_base, Eigen::Vector3d dc, Eigen::Vector3d centro_base);

	// Metodo para verificar se ocorre intersecao entre o raio e o cilindro
	double obter_ti(const Raio& raio);

	// Metodo para obter a normal do cilindro
	Eigen::Vector3d obter_normal(Eigen::Vector3d pontoIntersecao);

};

# endif