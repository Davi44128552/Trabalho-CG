#include "Cena.h"
#include "Material.h"


Cena::Cena(const Eigen::Vector3d& posicao_luz) 
    : posicao_luz(posicao_luz) {
    Eigen::Vector3d ka(0.1, 0.1, 0.1);
    Eigen::Vector3d kd(0.7, 0.2, 0.2);
    Eigen::Vector3d ks(1.0, 1.0, 1.0);
    Eigen::Vector3d cor(255, 80, 80);
     double shininess = 32.0;
    Material material(ka, kd, ks, shininess);
    // Inicializando a cena 
    objetos.push_back(new Esfera(Eigen::Vector3d(1.0, 0.0, -3.4), 0.4, material, cor )); 
    //objetos.push_back(new Esfera(Eigen::Vector3d(0.0, 0.0, -3.4), 0.4));
    //objetos.push_back(new Plano(Eigen::Vector3d(0.0, -0.5, 0.0), Eigen::Vector3d(0.0, 1.0, 0.0))); 
    //objetos.push_back(new Plano(Eigen::Vector3d(0.0, 1, 0.0), Eigen::Vector3d(0.0, -1.0, 0.0)));
    //objetos.push_back(new Cilindro(1, 1, Eigen::Vector3d(0.0, 1.0, 0.0), Eigen::Vector3d(0.0, 0.5, -4.4))); // Adicionado
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
    double t_min = std::numeric_limits<double>::infinity();
    Forma* objetoMaisProximo = nullptr;
    
    // Encontrar o objeto mais próximo
    for (auto& objeto : objetos) {
        double t = objeto->obter_ti(raio);
        if (t > 0.0001 && t < t_min) {
            t_min = t;
            objetoMaisProximo = objeto;
        }
    }
    
    if (!objetoMaisProximo) {
        return; // Nenhuma interseção encontrada, o pixel já está com a cor do background
    }
    
    // Ponto de interseção
    Eigen::Vector3d pontoIntersecao = raio.obter_origem() + t_min * raio.obter_direcao();
    Eigen::Vector3d normal = objetoMaisProximo->obter_normal(pontoIntersecao).normalized();
    Material material = objetoMaisProximo->getMaterial();
    
    // Cor inicial com iluminação ambiente
    Eigen::Vector3d corFinal = material.getKa().cwiseProduct(Eigen::Vector3d(0.3, 0.3, 0.3));
    
    // Vetor de visão (direção inversa do raio)
    Eigen::Vector3d visao = -raio.obter_direcao().normalized();
    
    // Raio de sombra: verificar se há um objeto bloqueando a luz
    Eigen::Vector3d direcaoLuz = (posicao_luz - pontoIntersecao).normalized();
    Raio raioLuz(pontoIntersecao + normal * 0.001, direcaoLuz);
    
    for (auto& objeto : objetos) {
        if (objeto == objetoMaisProximo) continue;
        double tLuz = objeto->obter_ti(raioLuz);
        if (tLuz > 0.0001 && tLuz < (posicao_luz - pontoIntersecao).norm()) {
            SDL_SetRenderDrawColor(renderer, corFinal[0] * 255, corFinal[1] * 255, corFinal[2] * 255, 255);
            SDL_RenderDrawPoint(renderer, colunas, linhas);
            return; // Está na sombra, apenas iluminação ambiente
        }
    }
    
    // Reflexão difusa
    double fatorDifuso = std::max(0.0, normal.dot(direcaoLuz));
    corFinal += fatorDifuso * material.getKd().cwiseProduct(Eigen::Vector3d(0.7, 0.7, 0.7));
    
    // Reflexão especular
    Eigen::Vector3d reflexao = (2.0 * fatorDifuso * normal - direcaoLuz).normalized();
    double fatorEspecular = std::pow(std::max(0.0, reflexao.dot(visao)), material.getShininess());
    corFinal += fatorEspecular * material.getKs().cwiseProduct(Eigen::Vector3d(1.0, 1.0, 1.0));
    
    // Garantir que a cor está no intervalo válido
    corFinal = corFinal.cwiseMin(1.0).cwiseMax(0.0);
    
    // Desenha o pixel
    SDL_SetRenderDrawColor(renderer, corFinal[0] * 255, corFinal[1] * 255, corFinal[2] * 255, 255);
    SDL_RenderDrawPoint(renderer, colunas, linhas);
}
