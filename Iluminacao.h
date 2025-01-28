# ifndef ILUMINACAO_H
# define ILUMINACAO_H

# include <Eigen/Dense>
# include <cmath>

// Criando a classe de intensidade
class Iluminacao{
	private:
		// Atributos 
			// Valores referente a intensidade Ambiente
		Eigen::Vector3d iluminacao_Ambiente;
		Eigen::Vector3d intensidade_Ambiente;
		Eigen::Vector3d k_Ambiente;

			// Valores referentes a intensidade Difusa
		Eigen::Vector3d iluminacao_Difusa;
		Eigen::Vector3d intensidade_Difusa;
		Eigen::Vector3d k_Difusa;

			// Valores referentes a intensidade Especular
		Eigen::Vector3d iluminacao_Especular;
		Eigen::Vector3d intensidade_Especular;
		Eigen::Vector3d k_Especular;
		double shininess;

			// O valor da intensidade total
		Eigen::Vector3d iluminacao_Total;

	public:
		// Funcoes que usaremos para calcular nossa intensidade
		// Construtor
		Iluminacao(const Eigen::Vector3d& intensidade_Ambiente, const Eigen::Vector3d& k_Ambiente, 
            const Eigen::Vector3d& intensidade_Difusa, const Eigen::Vector3d& k_Difusa, 
            const Eigen::Vector3d& intensidade_Especular, const Eigen::Vector3d& k_Especular, double shininess);

		// Funcao para calcular a iluminacao ambiente
		void calcular_iluminacao_Ambiente();

		// Funcao para calcular a iluminacao difusa
		void calcular_iluminacao_Difusa(const Eigen::Vector3d& luz, const Eigen::Vector3d& normal);

		// Funcao para calcular a iluminacao especular
		void calcular_iluminacao_Especular(const Eigen::Vector3d& luz, const Eigen::Vector3d& normal, const Eigen::Vector3d& visao);

		// Funcao para calcular a iluminacao total
		Eigen::Vector3d calcular_iluminacao_Total(const Eigen::Vector3d& luz, const Eigen::Vector3d& normal, const Eigen::Vector3d& visao);

		// Funcao para retornar a iluminacao ambiente
		Eigen::Vector3d retornar_iluminacao_Ambiente();

};

# endif