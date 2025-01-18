#ifndef CENA_H
#define CENA_H

#include <memory>
#include "Forma.h"
#include "Iluminacao.h"
#include "Raio.h"

class Cena {
public:
    std::vector<std::unique_ptr<Forma>> objetos;
    std::vector<Iluminacao> luzes;
    Eigen::Vector3d luz_ambiente;

    Cena();
    Cena(Eigen::Vector3d luz_ambiente);

    void adicionar_objeto(std::unique_ptr<Forma> obj);
    bool remover_objeto(Forma* obj);

    void adicionar_luz(const Iluminacao& l);
    bool remover_luz(const Iluminacao& l);

    std::tuple<Forma*, double> obter_objeto_mais_proximo(const Raio& raio);
};

#endif // CENA_H
