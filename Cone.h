# ifndef Cone_H
# define Cone_H

# include <Eigen/Dense>
# include <cmath>
# include "Raio.h"
# include "Forma.h"
# include "Material.h"

// Classe do Cone
class Cone : public Forma {

public:
	// Construtor do objeto
	Cone(double altura, double raio_base, Eigen::Vector3d dc, Eigen::Vector3d centro_base, const Material& material);

	// Metodo para verificar se ocorre intersecao entre o raio e o Cone
	Intersecao obter_intersecao(const Raio& raio) const override;

    // Metodo para calcular o centroide de um cone
	Eigen::Vector3d calcularCentroide() const;

    // Funcoes de transformacoes
    void translacao(Eigen::Vector3d d);
    void escalonar(double d);
    void rotacionar_eixo(char eixo, double angulo);
    void rotacionar_quaternio(double angulo, Eigen::Vector3d eixo);

private:
	// Atributos do Cone
	double altura; // Altura do Cone
	double raio_base; // Tamanho do raio da base
	Eigen::Vector3d dc; // Vetor unitario que indica a direcao do Cone
	Eigen::Vector3d centro_base; // Posicao no espaco em que se encontra o centro da base da esfera
};
# endif // Cone_H
