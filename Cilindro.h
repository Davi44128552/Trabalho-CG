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
	Intersecao obter_intersecao(const Raio& raio) const override;

private:
	// Atributos do cilindro
	double altura; // Altura do cilindro
	double raio_base; // Tamanho do raio da base
	Eigen::Vector3d dc; // Vetor unitario que indica a direcao do cilindro
	Eigen::Vector3d centro_base; // Posicao no espaco em que se encontra o centro da base da esfera


};
# endif // CILINDRO_H
