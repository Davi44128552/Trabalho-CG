# include "Cone.hpp"

// Implementando o construtor
Cone::Cone(Eigen::Vector3d vertice, Eigen::Vector3d dc, double altura, double raio_base){

	this->vertice = vertice;
	this->dc = dc.normalized();
	this->altura = altura;
	this->raio_base = raio_base;
	this->posicao = 'i'; // i de indefinido

}

// Implementando a funcao que verifica se ocorre intersecao entre o raio do observador e o objeto
double Cone::obter_ti(const Raio& raio){

	double cos2theta = pow((altura / sqrt(altura*altura + raio_base*raio_base)), 2);
	const double epsilon = 0.000001;

	// Definindo as variaveis para o calculo do discriminante
	double a = pow(dc.dot(raio.dr), 2) - raio.dr.dot(raio.dr) * cos2theta;
	double b = 2 * (vertice.dot(raio.dr) * cos2theta - vertice.dot(dc) * dc.dot(raio.dr));
	double c = vertice.dot(dc) * vertice.dot(dc) - vertice.dot(vertice) * cos2theta;

	// Calculando o valor de t, baseado na equacao do segundo grau
	double discriminante = b*b - 4 * a * c;

	// Verificando se o dicriminante é válido
	if (discriminante < 0){
		return nan("");
	}

	// Calculando as raizes da equacao
	double t1 = (-b -sqrt(discriminante)) / (2*a);
	double t2 = (-b + sqrt(discriminante)) / (2*a);

	// Verificando se o ponto nao esta atras da origem do raio
	if (t1 < 0 && t2 < 0){
		return nan("");
	}

	
	double t = std::min(t1 > 0 ? t1 : std::numeric_limits<double>::infinity(), 
                    t2 > 0 ? t2 : std::numeric_limits<double>::infinity());

	if (t == std::numeric_limits<double>::infinity()){
		return nan("");
	}

	Eigen::Vector3d ponto_intersecao = raio.Po + raio.dr * t;

	// Verificando se o ponto esta de fato no cone
	double verificacao = 0 <= dc.dot(ponto_intersecao - vertice) && dc.dot(ponto_intersecao - vertice) <= altura;
	if (verificacao){
		if (dc.dot(ponto_intersecao - vertice) < epsilon){
		posicao = 'b';
	}

	else{
		posicao = 'm';
	}
		return t;
	}

	t = std::max(t1, t2);
	ponto_intersecao = raio.Po + raio.dr * t;
	verificacao = 0 <= dc.dot(ponto_intersecao - vertice) && dc.dot(ponto_intersecao - vertice) <= altura;

	if (dc.dot(ponto_intersecao - vertice) < epsilon){
		posicao = 'b';
	}

	else{
		posicao = 'm';
	}

	if (verificacao){
		return t;
	}

	// Caso nada tenha dado certo para a verificacao, retornamos nulo
	return nan("");

}

// Implementando a função para obter a normal do cone
Eigen::Vector3d Cone::obter_normal(Eigen::Vector3d ponto_intersecao) {
    // Caso estejamos na base do cone, a sua normal simplesmente será -dc
    if (posicao == 'b') {
        return -dc.normalized();
    }

    // Caso estejamos na superfície lateral do cone
    if (posicao == 'm') {
        // Vetor que vai do vértice até o ponto de interseção
        Eigen::Vector3d vetor_vertice_ponto = ponto_intersecao - vertice;

        // Projeta o vetor_vertice_ponto na direção do eixo do cone (dc)
        double proj = vetor_vertice_ponto.dot(dc);

        // Calcula a projeção ao longo de dc
        Eigen::Vector3d proj_dc = proj * dc;

        // A componente perpendicular ao eixo do cone
        Eigen::Vector3d componente_perpendicular = vetor_vertice_ponto - proj_dc;

        double inclinacao = raio_base / altura;
		Eigen::Vector3d normal = componente_perpendicular - inclinacao * proj_dc;

        // Normalizamos o vetor resultante
        return normal.normalized();
    }

    // Caso a posição seja indefinida, retornamos um vetor NaN
    return Eigen::Vector3d::Constant(std::nan(""));
}
