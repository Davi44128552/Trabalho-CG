# include "Triangulo.h"

// Implementando o construtor
Triangulo::Triangulo(Vector3d vertice1, Vector3d vertice2, Vector3d vertice3){

	// Atribuindo os valores
	this->vertice1 = vertice1;
	this->vertice2 = vertice2;
	this->vertice3 = vertice3;

	// Chamando a funcao para calcular a normal do triangulo
	calcular_normal();

}

// Implementando a funcao para verificar a intersecao do objeto com o raio
double Triangulo::obter_ti(const Raio& raio){

	/* Obtendo o valor de v que, assim como o plano, e definido por raio.Po - vertice_qualquer
	   Como o vertice e um ponto conhecido, entao podemos usa-lo */
	Vector3d v = raio.Po - vertice1; // Usando o vertice 1

	// Usando uma precisao
	double epsilon = 0.000001;

	// Verificando se ha intersecao
	if (abs(raio.dr.dot(normal)) < epsilon){
		return nan(""); // Retornando nulo, caso o denominador seja invalido
	}

	// Obtendo o valor de t
	double t = - (v.dot(normal) / raio.dr.dot(normal));

	// Verificando se t e invalido
	if (t < 0){
		return nan("");
	}

	// Verificando se o ponto de intersecao com t e valido
	Vector3d ponto_intersecao = raio.Po + t*raio.dr;

	// Arestas do triangulo
	Vector3d aresta1 = vertice2 - vertice1;
	Vector3d aresta2 = vertice3 - vertice2;
	Vector3d aresta3 = vertice1 - vertice3;

	// valor booleano para confirmar se o ponto esta do lado de fora do triangulo
	bool fora_triangulo = 
	normal.dot(aresta1.cross(ponto_intersecao - vertice1)) < epsilon || 
	normal.dot(aresta2.cross(ponto_intersecao - vertice2)) < epsilon || 
	normal.dot(aresta3.cross(ponto_intersecao - vertice3)) < epsilon;

	if (fora_triangulo) {
		return nan("");
	}

	return t;

}

// Implementando a funcao para obter a normal do triangulo
Vector3d Triangulo::obter_normal(){
	return normal;
}

// Implementando a funcao para calcular a normal do triangulo
void Triangulo::calcular_normal(){

	// Calculando dois vetores a partir dos pontos
	Vector3d vetor1 = vertice2 - vertice1;
	Vector3d vetor2 = vertice3 - vertice1;

	// Apos obter os dois vertices, realizamos um produto vetorial deles
	normal = vetor1.cross(vetor2);
	normal = normal.normalized(); // Normalizando o vetor

}