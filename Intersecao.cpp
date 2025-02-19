#include "Intersecao.h"
#include "Eigen/src/Core/Matrix.h"


Intersecao::Intersecao(const bool existe, const double t, const Eigen::Vector3d normal, const Eigen::Vector3d ponto_intersecao)
    : t(t), normal(normal), existe(existe), ponto_intersecao(ponto_intersecao) {};

const Intersecao Intersecao::NONE = Intersecao(false, INFINITY, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(0.0, 0.0, 0.0));
