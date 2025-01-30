#include "Cena.h"


Cena::Cena(const Eigen::Vector3d& posicao_luz, const Iluminacao& iluminacao) 
    : posicao_luz(posicao_luz), iluminacao(iluminacao) {

    // Inicializando a cena 
    objetos.push_back(new Esfera(Eigen::Vector3d(1.0, 0.0, -3.4), 0.4)); 
    //objetos.push_back(new Esfera(Eigen::Vector3d(0.0, 0.0, -3.4), 0.4));
    objetos.push_back(new Plano(Eigen::Vector3d(0.0, -0.5, 0.0), Eigen::Vector3d(0.0, 1.0, 0.0))); 
    //objetos.push_back(new Plano(Eigen::Vector3d(0.0, 1, 0.0), Eigen::Vector3d(0.0, -1.0, 0.0)));
    objetos.push_back(new Cilindro(1, 1, Eigen::Vector3d(0.0, 1.0, 0.0), Eigen::Vector3d(0.0, 0.5, -4.4))); // Adicionado
}

void Cena::renderizar(SDL_Renderer* renderer, int nColunas, int nLinhas, const Eigen::Vector3d& posicao_observador, 
                      const Eigen::Vector3d& ponto_superior_esquerdo, double delta_x, double delta_y, double z) {
    for (int linhas = 0; linhas < nLinhas; linhas++) {
        double y = ponto_superior_esquerdo[1] - delta_y / 2 - delta_y * linhas;

        for (int colunas = 0; colunas < nColunas; colunas++) {
            double x = ponto_superior_esquerdo[0] + delta_x / 2 + delta_x * colunas;

            // Definindo o vetor dr e o raio para verificar as colisoes
            Eigen::Vector3d dr(x, y, -z);
            Raio raio(posicao_observador, dr);

            // Pintando a tela inteira de cinza para depois verificar a colisao dos objetos
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawPoint(renderer, colunas, linhas);

            // Calcular a iluminação e sombreamento
            calcularIluminacao(raio, renderer, colunas, linhas, x, y);
        }
    }

    SDL_RenderPresent(renderer);
}

void Cena::calcularIluminacao(Raio& raio, SDL_Renderer* renderer, int colunas, int linhas, double x, double y) {
    double ponto_mais_proximo = std::numeric_limits<double>::infinity();
    Forma* forma_interseccionada = nullptr;

    // Verificando interseção com os objetos da cena (esfera, plano, etc.)
    for (auto& objeto : objetos) {
        double ponto = objeto->obter_ti(raio);
        if (!isnan(ponto) && ponto < ponto_mais_proximo) {
            ponto_mais_proximo = ponto;
            forma_interseccionada = objeto;
        }
    }

    if (forma_interseccionada != nullptr) {
        Eigen::Vector3d ponto_intersecao = raio.obter_origem() + raio.obter_direcao() * ponto_mais_proximo;
        Eigen::Vector3d normal = forma_interseccionada->obter_normal(ponto_intersecao).normalized();
        Eigen::Vector3d cor;

        // Definindo a cor do objeto (exemplo simples)
        if (dynamic_cast<Esfera*>(forma_interseccionada)) {
            cor = Eigen::Vector3d(255, 80, 80);  // Cor da esfera
        } else if (dynamic_cast<Plano*>(forma_interseccionada)) {
            cor = Eigen::Vector3d(80, 80, 255);  // Cor do plano
        } else if (dynamic_cast<Cilindro*>(forma_interseccionada)) {
            cor = Eigen::Vector3d(80, 255, 80);  // Cor do cilindro
        }

        // Vetor de visão
        Eigen::Vector3d visao = -raio.obter_direcao();

        // Calculando a iluminação do ponto de interseção
        Eigen::Vector3d luz = (posicao_luz - ponto_intersecao).normalized();
        Raio raio_sombra(ponto_intersecao + luz * 1e-4, luz); // Raio para verificar sombra

        double ponto_sombra = std::numeric_limits<double>::infinity();
        for (auto& objeto : objetos) {
            double ponto = objeto->obter_ti(raio_sombra);
            if (!isnan(ponto) && ponto > 0 && ponto < (posicao_luz - ponto_intersecao).norm()) {
                ponto_sombra = ponto;
                break;
            }
        }

        Eigen::Vector3d iluminacao_total;
        if (ponto_sombra == std::numeric_limits<double>::infinity()) {
            iluminacao_total = iluminacao.calcular_iluminacao_Total(luz, normal, visao);
        } else {
            iluminacao_total = iluminacao.retornar_iluminacao_Ambiente();
        }

        // Aplicando a cor com base na iluminação
        iluminacao_total = iluminacao_total.cwiseProduct(cor);
        Uint8 r = static_cast<Uint8>(std::clamp(iluminacao_total[0], 0.0, 255.0));
        Uint8 g = static_cast<Uint8>(std::clamp(iluminacao_total[1], 0.0, 255.0));
        Uint8 b = static_cast<Uint8>(std::clamp(iluminacao_total[2], 0.0, 255.0));

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawPoint(renderer, colunas, linhas);
    }
}
