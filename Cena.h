#ifndef CENA_H
#define CENA_H

#include <vector>
#include <tuple>
#include <Eigen/Dense>
#include "Forma.h"
#include "Luz.h"
#include "Raio.h"

class Cena {
public:
    std::vector<Forma*> objects;
    std::vector<Luz> lights;
    Eigen::Vector3d ambient_light;

    Cena();
    Cena(Eigen::Vector3d ambient_light);

    void add_object(Forma* obj);
    void remove_object(int n);
    bool remove_object(Forma* obj);

    std::tuple<Forma*, Intersecao> get_closest_object(Raio ray);

    void add_light(Luz l);
    void remove_light(int n);
    bool remove_light(Luz l);

};

#endif // CENA_H
