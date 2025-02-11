# ifndef MALHA_H
# define MALHA_H

# include <Eigen/Dense>
# include <vector>
# include <algorithm>
# include "Raio.hpp"
# include "Triangulo.hpp"
//# include "Aresta.h"

using namespace Eigen;

// Classe da malha
class Malha{

private:
	// Atributos
	int id_triangulo_atual;
	std::vector<Triangulo> faces;
	//std::vector<Aresta> arestas;
	std::vector<Vector3d> vertices;
	Vector3d normal;

public:
	// Construtor da malha
	Malha(std::vector<Triangulo> faces, std::vector<Vector3d> vertices);

	// Funcao para obter o ponto de intersecao
	double obter_ti(const Raio& raio);

	// Funcao para calcular a normal
	Vector3d obter_normal();

};

# endif