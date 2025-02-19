#include "Cena.h"
#include "Intersecao.h"

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

std::tuple<Forma*, Intersecao> Cena::get_closest_object(Raio ray) {
    Intersecao min_i = Intersecao::NONE;
    Forma* best_shape = nullptr;

    for (Forma* s_candidate : objects) {
        Intersecao i_candidate = s_candidate->obter_intersecao(ray);
        if (i_candidate.existe && i_candidate.t >= 0.0 && i_candidate.t < min_i.t) {
            min_i = i_candidate;
            best_shape = s_candidate;
        }
    }

    return std::make_tuple(best_shape, min_i);
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
