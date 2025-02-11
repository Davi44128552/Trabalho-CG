# ifndef CILINDRO_H
# define CILINDRO_H

# include <Eigen/Dense>
# include <cmath>
# include "Raio.h"
# include "Forma.h"
# include "Material.h"

// Classe do cilindro
class Cilindro : public Forma {

public:
	// Construtor do objeto
	Cilindro(double altura, double raio_base, Eigen::Vector3d dc, Eigen::Vector3d centro_base, const Material& material);

	// Metodo para verificar se ocorre intersecao entre o raio e o cilindro
	double obter_ti(const Raio& raio) const override;

	// Metodo para obter a normal do cilindro
	Eigen::Vector3d obter_normal(const Eigen::Vector3d& ponto) const override;

private:
	// Atributos do cilindro
	double altura; // Altura do cilindro
	double raio_base; // Tamanho do raio da base
	Eigen::Vector3d dc; // Vetor unitario que indica a direcao do cilindro
	Eigen::Vector3d centro_base; // Posicao no espaco em que se encontra o centro da base da esfera
	mutable char posicao; // Valor que vai definir se estamos no topo, base ou no meio do cilindro

};
# endif // CILINDRO_H