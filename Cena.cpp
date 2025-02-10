#include "Cena.h"
#include <limits>

Cena::Cena() : objects(std::vector<Forma*>()), lights(std::vector<Luz>()), ambient_light(Eigen::Vector3d(1.0, 1.0, 1.0)) {}

Cena::Cena(Eigen::Vector3d ambient_light) : objects(std::vector<Forma*>()), lights(std::vector<Luz>()), ambient_light(ambient_light) {}

void Cena::add_object(Forma* obj) {
    objects.push_back(obj);
}

void Cena::remove_object(int n) {
    objects.erase(objects.begin() + n);
}

bool Cena::remove_object(Forma* obj) {
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i] == obj) {
            objects.erase(objects.begin() + i);
            return true;
        }
    }
    return false;
}

std::tuple<Forma*, double> Cena::get_closest_object(Raio ray) {
    double min_t = std::numeric_limits<double>::infinity();
    Forma* best_shape = nullptr;

    for (Forma* s_candidate : objects) {
        double t_candidate = s_candidate->obter_ti(ray);
        if (t_candidate >= 0.0 && t_candidate < min_t) {
            min_t = t_candidate;
            best_shape = s_candidate;
        }
    }

    if (min_t == std::numeric_limits<double>::infinity()) {
        min_t = -std::numeric_limits<double>::infinity();
    }

    return std::make_tuple(best_shape, min_t);
}

void Cena::add_light(Luz l) {
    lights.push_back(l);
}

void Cena::remove_light(int n) {
    lights.erase(lights.begin() + n);
}

bool Cena::remove_light(Luz l) {
    for (size_t i = 0; i < lights.size(); i++) {
        if (lights[i].posicao == l.posicao && lights[i].cor == l.cor && lights[i].intensidade == l.intensidade) {
            lights.erase(lights.begin() + i);
            return true;
        }
    }
    return false;
}
