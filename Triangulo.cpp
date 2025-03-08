# include "Triangulo.h"
#include <Eigen/Geometry> // Para usar quatérnios do Eigen

// Implementando o construtor
Triangulo::Triangulo(Eigen::Vector3d vertice1, Eigen::Vector3d vertice2, Eigen::Vector3d vertice3)
	: vertice1(vertice1), vertice2(vertice2), vertice3(vertice3)
	{
		normal = obter_normal();
	}

// Implementando a funcao para verificar a intersecao do objeto com o raio
double Triangulo::obter_ti(const Raio& raio) const {

	/* Obtendo o valor de v que, assim como o plano, e definido por raio.Po - vertice_qualquer
	   Como o vertice e um ponto conhecido, entao podemos usa-lo */
	Eigen::Vector3d v = raio.Po - vertice1; // Usando o vertice 1

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
	Eigen::Vector3d ponto_intersecao = raio.Po + t*raio.dr;

	// Arestas do triangulo
	Eigen::Vector3d aresta1 = vertice2 - vertice1;
	Eigen::Vector3d aresta2 = vertice3 - vertice2;
	Eigen::Vector3d aresta3 = vertice1 - vertice3;

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
Eigen::Vector3d Triangulo::obter_normal() const {
    Eigen::Vector3d vetor1 = vertice2 - vertice1;
    Eigen::Vector3d vetor2 = vertice3 - vertice1;
    return vetor1.cross(vetor2).normalized(); // Retorna diretamente o valor
}

// Implementando a funcao para realizar a translacao
void Triangulo::translacao(Eigen::Vector3d d){
	vertice1 = vertice1+d;
	vertice2 = vertice2+d;
	vertice3 = vertice3+d;
}

// Implementando a funcao para realizar a rotacao
void Triangulo::rotacionar_quaternio(double angulo, Eigen::Vector3d eixo) {
	// Convertendo o ângulo de graus para radianos
	double anguloRad = angulo * M_PI / 180.0;

	// Normalizando o eixo de rotação
	eixo.normalize();

	// Criando o quatérnio de rotação
	Eigen::Quaterniond q;
	q = Eigen::AngleAxisd(anguloRad, eixo);

	// Aplicando a rotação aos vértices
	vertice1 = q * vertice1;
	vertice2 = q * vertice2;
	vertice3 = q * vertice3;

	// Atualizando a normal do triângulo
	normal = obter_normal();
}
void Triangulo::rotacionar_eixo(char eixo, double angulo){

  // Definindo a matriz
  Eigen::Matrix3d K;

  // Convertendo o angulo para radianos
  double anguloRad = angulo * M_PI / 180.0;

  // Verificando em torno de qual eixo sera rotacionado
  if (eixo == 'x'){
    K <<    1, 0, 0,
            0, cos(anguloRad), -sin(anguloRad),
            0, sin(anguloRad), cos(anguloRad);
  }

  else if (eixo == 'y'){
    K <<    cos(anguloRad), 0, sin(anguloRad),
            0, 1, 0,
            -sin(anguloRad), 0, cos(anguloRad);
  }

  else if (eixo == 'z'){
    K <<    cos(anguloRad), -sin(anguloRad), 0,
            sin(anguloRad), cos(anguloRad), 0,
            0, 0, 1;
  }

  else{
    return; // Para casos invalidos
  }

  // Reposicionando os vertices de acordo com as matrizes
  vertice1 = K * vertice1;
  vertice2 = K * vertice2;
  vertice3 = K * vertice3;

  // Atualizando a normal
  normal = obter_normal();
}

void Triangulo::escalonar(Eigen::Vector3d s){
    // Calculando o vetor de translacao para a origem (subtraindo o centroide)
    Eigen::Vector3d centroide = (vertice1 + vertice2 + vertice3) / 3.0;

    // Matrizes de transformacao para transladar, escalar e transladar de volta

    // Matriz para escalonar o nosso objeto
    Eigen::Matrix4d matriz_escala;
    matriz_escala << s.x(), 0,  0,  0,
                   0, s.y(), 0,  0,
                   0, 0, s.z(), 0,
                   0, 0, 0, 1;


    // Multiplicando as matrizes para a transformação final
    Eigen::Matrix4d transformationMatrix = matriz_escala;

    // Aplicando a transformacao nos vertices
    	// Nossos vertices serao atualizados para R4
    Eigen::Vector4d v1_alterado(vertice1.x(), vertice1.y(), vertice1.z(), 1);
    Eigen::Vector4d v2_alterado(vertice2.x(), vertice2.y(), vertice2.z(), 1);
    Eigen::Vector4d v3_alterado(vertice3.x(), vertice3.y(), vertice3.z(), 1);

    v1_alterado = transformationMatrix * v1_alterado;
    v2_alterado = transformationMatrix * v2_alterado;
    v3_alterado = transformationMatrix * v3_alterado;

    // Atualizando os vertices com os novos valores
    vertice1 = Eigen::Vector3d(v1_alterado.x(), v1_alterado.y(), v1_alterado.z());
    vertice2 = Eigen::Vector3d(v2_alterado.x(), v2_alterado.y(), v2_alterado.z());
    vertice3 = Eigen::Vector3d(v3_alterado.x(), v3_alterado.y(), v3_alterado.z());

    // Atualizando a normal apos a transformação
    normal = obter_normal();
}

void Triangulo::cisalhar(double shXY, double shXZ, double shYX, double shYZ, double shZX, double shZY) {

    // Matriz de cisalhamento
    Eigen::Matrix4d shearMatrix;
    shearMatrix << 1, shXY, shXZ, 0,
                   shYX, 1, shYZ, 0,
                   shZX, shZY, 1, 0,
                   0, 0, 0, 1;


    // Multiplicando as matrizes para obter a matriz de transformação final
    Eigen::Matrix4d transformationMatrix = shearMatrix;

    // Aplicando a transformação nos vértices
    Eigen::Vector4d v1Transformed(vertice1.x(), vertice1.y(), vertice1.z(), 1);
    Eigen::Vector4d v2Transformed(vertice2.x(), vertice2.y(), vertice2.z(), 1);
    Eigen::Vector4d v3Transformed(vertice3.x(), vertice3.y(), vertice3.z(), 1);

    v1Transformed = transformationMatrix * v1Transformed;
    v2Transformed = transformationMatrix * v2Transformed;
    v3Transformed = transformationMatrix * v3Transformed;

    // Atualizando os vértices com os novos valores
    vertice1 = Eigen::Vector3d(v1Transformed.x(), v1Transformed.y(), v1Transformed.z());
    vertice2 = Eigen::Vector3d(v2Transformed.x(), v2Transformed.y(), v2Transformed.z());
    vertice3 = Eigen::Vector3d(v3Transformed.x(), v3Transformed.y(), v3Transformed.z());

    // Atualizando a normal após a transformação
    normal = obter_normal();
}

