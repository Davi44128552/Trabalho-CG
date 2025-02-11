# include "Triangulo.hpp"

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

// Implementando a funcao para realizar a translacao
void Triangulo::translacao(Vector3d d){
	vertice1 = vertice1+d;
	vertice2 = vertice2+d;
	vertice3 = vertice3+d;
}

// Implementando a funcao para realizar a rotacao
void Triangulo::rotacionar(double angulo, Vector3d eixo) {
    // Convertendo o ângulo de graus para radianos
    double anguloRad = angulo * M_PI / 180.0;

    // Normalizando o eixo de rotacao
    eixo.normalize();

    // Calculando o centroide do triangulo
    Vector3d centroide = (vertice1 + vertice2 + vertice3) / 3.0;

    // Transladando os vertices para a origem
    vertice1 -= centroide;
    vertice2 -= centroide;
    vertice3 -= centroide;

    // Criando a matriz de rotacao usando a formula de Rodrigues
    Eigen::Matrix3d K;
    K <<    0, -eixo.z(), eixo.y(),
            eixo.z(), 0, -eixo.x(),
            -eixo.y(), eixo.x(), 0;

    Eigen::Matrix3d R = Eigen::Matrix3d::Identity() + sin(anguloRad) * K + (1 - cos(anguloRad)) * (K * K);

    // Aplicando a rotacao aos vertices
    vertice1 = R * vertice1;
    vertice2 = R * vertice2;
    vertice3 = R * vertice3;

    // Transladando os vertices de volta para a posicao original
    vertice1 += centroide;
    vertice2 += centroide;
    vertice3 += centroide;

    // Atualizando a normal do triângulo
    calcular_normal();
}

void Triangulo::escalonar(Vector3d s){
    // Calculando o vetor de translacao para a origem (subtraindo o centroide)
    Vector3d centroide = (vertice1 + vertice2 + vertice3) / 3.0;

    // Matrizes de transformacao para transladar, escalar e transladar de volta

    // Matrizes de translacao para a origem
    Eigen::Matrix4d trans_origem;
    trans_origem << 1, 0, 0, -centroide.x(),
                     0, 1, 0, -centroide.y(),
                     0, 0, 1, -centroide.z(),
                     0, 0, 0, 1;

    // Matriz para escalonar o nosso objeto
    Eigen::Matrix4d matriz_escala;
    matriz_escala << s.x(), 0,  0,  0,
                   0, s.y(), 0,  0,
                   0, 0, s.z(), 0,
                   0, 0, 0, 1;

    // Matrizes de translacao para voltar ao seu ponto anterior
    Eigen::Matrix4d trans_voltar;
    trans_voltar << 1, 0, 0, centroide.x(),
                 0, 1, 0, centroide.y(),
                 0, 0, 1, centroide.z(),
                 0, 0, 0, 1;

    // Multiplicando as matrizes para a transformação final
    Eigen::Matrix4d transformationMatrix = trans_voltar * matriz_escala * trans_origem;

    // Aplicando a transformacao nos vertices
    	// Nossos vertices serao atualizados para R4
    Eigen::Vector4d v1_alterado(vertice1.x(), vertice1.y(), vertice1.z(), 1);
    Eigen::Vector4d v2_alterado(vertice2.x(), vertice2.y(), vertice2.z(), 1);
    Eigen::Vector4d v3_alterado(vertice3.x(), vertice3.y(), vertice3.z(), 1);

    v1_alterado = transformationMatrix * v1_alterado;
    v2_alterado = transformationMatrix * v2_alterado;
    v3_alterado = transformationMatrix * v3_alterado;

    // Atualizando os vertices com os novos valores
    vertice1 = Vector3d(v1_alterado.x(), v1_alterado.y(), v1_alterado.z());
    vertice2 = Vector3d(v2_alterado.x(), v2_alterado.y(), v2_alterado.z());
    vertice3 = Vector3d(v3_alterado.x(), v3_alterado.y(), v3_alterado.z());

    // Atualizando a normal apos a transformação
    calcular_normal();
}

void Triangulo::cisalhar(double shXY, double shXZ, double shYX, double shYZ, double shZX, double shZY) {
    // Calculando o centroide do triângulo
    Vector3d centroide = (vertice1 + vertice2 + vertice3) / 3.0;

    // Matriz de translação para a origem
    Eigen::Matrix4d transToOrigin;
    transToOrigin << 1, 0, 0, -centroide.x(),
                     0, 1, 0, -centroide.y(),
                     0, 0, 1, -centroide.z(),
                     0, 0, 0, 1;

    // Matriz de cisalhamento
    Eigen::Matrix4d shearMatrix;
    shearMatrix << 1, shXY, shXZ, 0,
                   shYX, 1, shYZ, 0,
                   shZX, shZY, 1, 0,
                   0, 0, 0, 1;

    // Matriz de translação de volta
    Eigen::Matrix4d transBack;
    transBack << 1, 0, 0, centroide.x(),
                 0, 1, 0, centroide.y(),
                 0, 0, 1, centroide.z(),
                 0, 0, 0, 1;

    // Multiplicando as matrizes para obter a matriz de transformação final
    Eigen::Matrix4d transformationMatrix = transBack * shearMatrix * transToOrigin;

    // Aplicando a transformação nos vértices
    Eigen::Vector4d v1Transformed(vertice1.x(), vertice1.y(), vertice1.z(), 1);
    Eigen::Vector4d v2Transformed(vertice2.x(), vertice2.y(), vertice2.z(), 1);
    Eigen::Vector4d v3Transformed(vertice3.x(), vertice3.y(), vertice3.z(), 1);

    v1Transformed = transformationMatrix * v1Transformed;
    v2Transformed = transformationMatrix * v2Transformed;
    v3Transformed = transformationMatrix * v3Transformed;

    // Atualizando os vértices com os novos valores
    vertice1 = Vector3d(v1Transformed.x(), v1Transformed.y(), v1Transformed.z());
    vertice2 = Vector3d(v2Transformed.x(), v2Transformed.y(), v2Transformed.z());
    vertice3 = Vector3d(v3Transformed.x(), v3Transformed.y(), v3Transformed.z());

    // Atualizando a normal após a transformação
    calcular_normal();
}

