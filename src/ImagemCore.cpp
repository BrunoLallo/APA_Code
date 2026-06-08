#include <ImagemCore.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

Imagem carregarImagem(const std::string& caminho) {
    int w, h, canais;

    unsigned char* pixels = stbi_load(caminho.c_str(), &w, &h, &canais, 1);

    if(!pixels) {
        std::cerr << "Erro ao carregar a imagem!" << std::endl;
        std::cerr << "Erro: " << stbi_failure_reason() << "\n";
        exit(1);
    }

    Imagem img(w, h);
    for (int i = 0; i < w * h; i++) {
        img.dados[i] = (double)pixels[i] / 255.0;
    }

    stbi_image_free(pixels);
    return img;
}

void salvarImagem(const Imagem& img, const std::string& caminho) {
    std::vector<unsigned char> pixels(img.largura * img.altura);

    for (int i = 0; i < img.largura * img.altura; i++) {
        double valor = img.dados[i];

        if (valor < 0.0) valor = 0.0;
        if (valor > 1.0) valor = 1.0;

        pixels[i] = static_cast<unsigned char>(valor * 255.0);
    }

    stbi_write_png(caminho.c_str(), img.largura, img.altura, 1, pixels.data(), img.largura);
    std::cout << "Imagem salva com sucesso em: " << caminho << "\n";
}

Imagem aplicarZeroPadding(const Imagem& img, int novaLargura, int novaAltura, int deslocamentoX, int deslocamentoY) {
    Imagem padded(novaLargura, novaAltura);

    for (int y = 0; y < img.altura; y++) {
        for (int x = 0; x < img.largura; x++) {
            int novoX = x + deslocamentoX;
            int novoY = y + deslocamentoY;

            if (novoX >= 0 && novoX < novaLargura && novoY >= 0 && novoY < novaAltura) {
                padded.at(novoX, novoY) = img.at(x, y);
            }
        }
    }

    return padded;
}

int proximaPotenciaDe2(int maiorLado) {
    return std::pow(2, std::ceil(std::log2(maiorLado)));
}