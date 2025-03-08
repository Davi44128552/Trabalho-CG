#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <vector>
#include <cstdint>
#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Texture.h"

class TextureLoader {
public:
    static Texture* loadTexture(const std::string& filePath) {
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (!surface) {
            std::cerr << "Erro ao carregar a imagem: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        uint32_t width = surface->w;
        uint32_t height = surface->h;
        uint32_t pitch = surface->pitch;
        uint32_t bpp = surface->format->BytesPerPixel;

        if (bpp < 3) {
            std::cerr << "Formato de imagem não suportado!" << std::endl;
            SDL_FreeSurface(surface);
            return nullptr;
        }

        uint8_t* pixels = static_cast<uint8_t*>(surface->pixels);
        std::vector<uint8_t> textureData(pixels, pixels + (height * pitch));

        SDL_FreeSurface(surface);

        return new Texture(textureData, width, height, pitch, bpp);
    }
};

#endif
