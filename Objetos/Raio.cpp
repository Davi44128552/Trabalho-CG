#include "Raio.hpp"

// Implementação do construtor
Raio::Raio(Eigen::Vector3d Po, Eigen::Vector3d dr) {
    this->Po = Po;
    this->dr = dr;
    this->P = Po + dr;
}
