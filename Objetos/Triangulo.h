# ifndef TRIANGULO_H
# define TRIANGULO_H

# include <Eigen/Dense>
# include "Raio.h"

using namespace Eigen;

// Classe do triangulo
class Triangulo{

private:
	// Atributos do triangulo
	// Os 3 vertices do triangulo
	Vector3d vertice1;
	Vector3d vertice2;
	Vector3d vertice3;

	// Normal do triangulo
	Vector3d normal;

public:
	// Construtor
	Triangulo(Vector3d vertice1, Vector3d vertice2, Vector3d vertice3);

	// Funcao para verificar se ocorre intersecao entre o raio e o objeto
	double obter_ti(const Raio& raio);

	// Funcao para obter a normal
	Vector3d obter_normal();

	// Funcao para CALCULAR a normal
	void calcular_normal();

	// Funcoes para realizar as transformacoes nos objetos
	void translacao(Vector3d d);
	void rotacionar(double angulo, Vector3d eixo);
	void escalonar(Vector3d s);
	void cisalhar(double shXY, double shXZ, double shYX, double shYZ, double shZX, double shZY);

};

# endif
