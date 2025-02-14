# ifndef MALHA_H
# define MALHA_H

# include <Eigen/Dense>
# include <vector>
# include <algorithm>
# include "Raio.h"
# include "Triangulo.h"
# include "Forma.h"
# include "Material.h"

// Classe da malha
class Malha : public Forma{

public:
	// Construtor da malha
	Malha(std::vector<Triangulo> faces, std::vector<Eigen::Vector3d> vertices, const Material& material);

	// Funcao para obter o ponto de intersecao
	double obter_ti(const Raio& raio) const override;

	// Funcao para calcular a normal
	Eigen::Vector3d obter_normal(const Eigen::Vector3d& ponto) const override;
	
private:
	// Atributos
	std::vector<Triangulo> faces;
	std::vector<Eigen::Vector3d> vertices;
	mutable Eigen::Vector3d normal;

};

# endif
