# ifndef TRIANGULO_H
# define TRIANGULO_H

# include <Eigen/Dense>
# include "Raio.h"
# include <cmath>


// Classe do triangulo
class Triangulo {

public:
	// Os 3 vertices do triangulo
	Eigen::Vector3d vertice1;
	Eigen::Vector3d vertice2;
	Eigen::Vector3d vertice3;

	// Construtor
	Triangulo(Eigen::Vector3d vertice1, Eigen::Vector3d vertice2, Eigen::Vector3d vertice3);

	// Funcao para verificar se ocorre intersecao entre o raio e o objeto
	double obter_ti(const Raio& raio) const;

	// Funcao para calcular e obter a normal
	Eigen::Vector3d obter_normal() const;

	// Funcoes para realizar as transformacoes nos objetos
	void translacao(Eigen::Vector3d d);
	void rotacionar(double angulo, Eigen::Vector3d eixo);
    void rotacionar_eixo(char eixo, double angulo);
	void escalonar(Eigen::Vector3d s);
	void cisalhar(double shXY, double shXZ, double shYX, double shYZ, double shZX, double shZY);
	
private:
	// Atributos do triangulo


	// Normal do triangulo
	Eigen::Vector3d normal;

};

# endif
