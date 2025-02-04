# include "Malha.h"

// Construtor
Malha::Malha(std::vector<Triangulo> faces, std::vector<Vector3d> vertices){

	this->id_triangulo_atual = 0;
	this->faces = faces;
	//this->arestas = arestas;
	this->vertices = vertices;

}

// Funcao para obter o ponto de intersecao do raio com algum triangulo da malha
double Malha::obter_ti(const Raio& raio){

	for (Triangulo face : faces){

		double ti = face.obter_ti(raio);
		// Verificando se este valor nao e nulo
		if (!std::isnan(ti)){

			// <----- RETOMAR PARA ENCONTRAR QUAL É A FACE QUE FOI INTERSECCIONADA
			return ti;
		}

	}

	return nan("");

}

// Funcao para calcular a normal da malha
Vector3d Malha::obter_normal(Vector3d ponto_intersecao){

	// Pegando o triangulo que guardamos o indice
	Triangulo Triangulo_atual = faces[id_triangulo_atual];

	// Calculando a sua normal
	Vector3d normal = Triangulo_atual.obter_normal();
	return normal;

}