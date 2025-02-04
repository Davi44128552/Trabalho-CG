# ifndef ARESTA_H
# define ARESTA_H

# include <Eigen/Dense>
using namespace Eigen;

class Aresta{

private:
	Vector3d vertice_1;
	Vector3d vertice_2;

public:
	// Construtor da funcao
	Aresta(Vector3d vertice_1, Vector3d vertice_2);

};

#endif