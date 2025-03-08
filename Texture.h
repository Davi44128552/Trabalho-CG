#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <Eigen/Dense>

class Texture {
public:
    Texture(const std::vector<uint8_t>& textureData, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp)
        : textureData(textureData), width(width), height(height), pitch(pitch), bpp(bpp) {}

    // Método para criar uma textura a partir de uma superfície 
    static Texture fromSurface(const std::vector<uint8_t>& surfaceData, uint32_t width, uint32_t height, uint32_t pitch) {
        uint32_t bpp = pitch / width; // Bytes por pixel
        return Texture(surfaceData, width, height, pitch, bpp);
    }

    // Método para amostrar a textura nas coordenadas (u, v)
    Eigen::Vector3d sample(double u, double v) const {
        // Garantir que u e v estejam no intervalo [0, 1]
        u = std::fmod(u, 1.0);
        v = std::fmod(v, 1.0);
        if (u < 0) u += 1.0;
        if (v < 0) v += 1.0;

        // Calcular as coordenadas (x, y) na textura
        uint32_t x = static_cast<uint32_t>(std::round(u * (width - 1)));
        uint32_t y = static_cast<uint32_t>(std::round(v * (height - 1)));

        // Calcular o índice no vetor de dados da textura
        size_t index = (y * pitch) + (x * bpp);

        // Verificar se o índice está dentro dos limites
        if (index + 2 >= textureData.size()) {
            throw std::out_of_range("Índice fora dos limites da textura.");
        }

        // Extrair os componentes de cor (R, G, B)
        uint8_t r = textureData[index];
        uint8_t g = textureData[index + 1];
        uint8_t b = textureData[index + 2];

        // Normalizar os valores para o intervalo [0, 1]
        return Eigen::Vector3d(
            static_cast<double>(r) / 255.0,
            static_cast<double>(g) / 255.0,
            static_cast<double>(b) / 255.0
        );
    }

    // Getters para as propriedades da textura
    uint32_t getWidth() const { return width; }
    uint32_t getHeight() const { return height; }
    uint32_t getPitch() const { return pitch; }
    uint32_t getBpp() const { return bpp; }

private:
    std::vector<uint8_t> textureData; // Dados da textura (R, G, B, ...)
    uint32_t width;                   // Largura da textura
    uint32_t height;                  // Altura da textura
    uint32_t pitch;                   // Tamanho de uma linha em bytes
    uint32_t bpp;                     // Bytes por pixel
};

#endif // TEXTURE_H