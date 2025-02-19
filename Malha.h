# ifndef MALHA_H
# define MALHA_H

# include <Eigen/Dense>
# include <vector>
# include <algorithm>
# include "Raio.h"
# include "Triangulo.h"
# include "Forma.h"
# include "Material.h"
# include "Intersecao.h"

// Classe da malha
class Malha : public Forma{

public:
	// Construtor da malha
	Malha(std::vector<Triangulo> faces, std::vector<Eigen::Vector3d> vertices, const Material& material);

	// Funcao para obter o ponto de intersecao
	Intersecao obter_intersecao(const Raio& raio) const override;

	// Funcoes das transformacoes
	void translacao(Eigen::Vector3d d);
	void rotacionar(double angulo, Eigen::Vector3d eixo);
	void escalonar(Eigen::Vector3d s);
	void cisalhar(double shXY, double shXZ, double shYX, double shYZ, double shZX, double shZY);
	
private:
	// Atributos
	std::vector<Triangulo> faces;
	std::vector<Eigen::Vector3d> vertices;

};

# endif
