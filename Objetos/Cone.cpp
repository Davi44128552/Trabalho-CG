# include "Cone.h"

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

	// Definindo a variavel v, que equivale a posicao do vertice menos Po do raio
	Eigen::Vector3d v = vertice - raio.Po;
	double cos2theta = pow((altura / sqrt(altura*altura + raio_base*raio_base)), 2);

	// Definindo as variaveis para o calculo do discriminante
	double a = pow(dc.dot(raio.dr), 2) - raio.dr.dot(raio.dr) * cos2theta;
	double b = 2 * (vertice.dot(raio.dr) * cos2theta - vertice.dot(dc) * dc.dot(raio.dr));
	double c = vertice.dot(dc) * vertice.dot(dc) - vertice.dot(vertice) * cos2theta;
	double epsilon = 0.000001;

	// Verificando se a e proximo do absoluto
	if (abs(a) > epsilon){

		// Verificando se b nao e zero, para evitar problemas
		if (b == 0){
			return nan("");
		}

		double t = - (c / (2*b));
		Eigen::Vector3d ponto_intersecao = raio.Po + t*raio.dr;
		bool teste_do_ponto = 0 <= (vertice - ponto_intersecao).dot(dc) && (vertice - ponto_intersecao).dot(dc) <= altura; // Verificando se o ponto e valido

		if (t > epsilon && teste_do_ponto ){ 
			return t;
		}
	}

	// Calculando o valor de t, baseado na equacao do segundo grau
	double discriminante = b*b - 4 * a * c;

	// Verificando o valor do discriminante
	if (discriminante < 0){
		return nan("");
	}

	// Calculando os valores para t
	double t1 = (-b -std::sqrt(discriminante)) / (2.0 * a);
	double t2 = (-b +std::sqrt(discriminante)) / (2.0 * a);

	// Verificando se as raizes da equacao sao validas
	for (double t: {t1, t2}){

		if (t > 0){

			// Verificando se 0 <= (vetice - P) * dc <= altura
			Eigen::Vector3d ponto_intersecao = raio.Po + t*raio.dr;
			if (0 <= (vertice - ponto_intersecao).dot(dc) && (vertice - ponto_intersecao).dot(dc) <= altura){

				// Definindo em que parte estamos do cone
				if ((vertice - ponto_intersecao).dot(dc) == 0){
					posicao = 'b'; // base
					std::cout << "Base" << "\n";
				}

				else{
					posicao = 'm'; // meio
					std::cout << "Meio" << "\n";
				}

				return t;

			}

		}

	}


	return nan("");

}

// Implementando a função para obter a normal do cone
Eigen::Vector3d Cone::obter_normal(Eigen::Vector3d ponto_intersecao) {
    // Caso estejamos na base do cone, a sua normal simplesmente será -dc
    if (posicao == 'b') {
        return -dc;
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
