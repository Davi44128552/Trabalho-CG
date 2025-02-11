# ifndef RAIO_H
# define RAIO_H

# include <Eigen/Dense>

class Raio {

public:
    // Atributos
    Eigen::Vector3d P;
    Eigen::Vector3d Po;
    Eigen::Vector3d dr;
    
    // Construtores
    Raio(Eigen::Vector3d Po, Eigen::Vector3d dr);
};

#endif // RAIO_H
