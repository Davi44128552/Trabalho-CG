#ifndef PLANO_H
#define PLANO_H

#include <Eigen/Dense>
#include "Raio.h"
#include "Forma.h"
# include "Material.h"

class Plano : public Forma {

private:
	// Atributos
	Eigen::Vector3d ponto_plano; // Um ponto conhecido do plano
	Eigen::Vector3d normal; // Normal do plano

public:
	// Metodos que serao usados
	// Construtor
	Plano(Eigen::Vector3d ponto_plano, Eigen::Vector3d normal, const Material& material);

	// Metodo para verificar se ocorre intersecao entre o raio e o plano
	double obter_ti(const Raio& raio) const override;

	// Metodo para obter a normal
	Eigen::Vector3d obter_normal(const Eigen::Vector3d& ponto) const override;

};

# endif
