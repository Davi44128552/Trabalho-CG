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
	std::pair<double, double> getTextureCoords(const Eigen::Vector3d& point) const override;

    // Metodo para obter centroide
	Eigen::Vector3d calcularCentroide() const;

    // Funcoes das transformacoes
    void translacao(Eigen::Vector3d d);
    void escalonar(double d);
    void rotacionar_eixo(char eixo, double angulo);
	void rotacionar_quaternio(double angulo, Eigen::Vector3d eixo);

private:
	// Atributos do cilindro
	double altura; // Altura do cilindro
	double raio_base; // Tamanho do raio da base
	Eigen::Vector3d dc; // Vetor unitario que indica a direcao do cilindro
	Eigen::Vector3d centro_base; // Posicao no espaco em que se encontra o centro da base da esfera


};
# endif // CILINDRO_H
