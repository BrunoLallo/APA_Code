#pragma once

#include <vector>
#include <complex>
#include <ImagemCore.h>

struct ImagemComplexa {
    int largura;
    int altura;
    std::vector<std::complex<double>> dados; // Guarda a imagem inteira na frequência

    ImagemComplexa() : largura(0), altura(0) {} 

    ImagemComplexa(int w, int h) : largura(w), altura(h), dados(w * h, std::complex<double>(0.0, 0.0)) {}

    ImagemComplexa(const std::vector<std::complex<double>>& vector, int w) : dados(vector), largura(w) {
        altura = dados.size() / largura;
    }

    ImagemComplexa(const Imagem& img) : largura(img.largura), altura(img.altura), dados(img.largura * img.altura) {
        for (int i = 0; i < dados.size(); i++) {
            dados[i] = std::complex<double>(img.dados[i], 0.0);
        }
    }

    // Atalho para acessar os índices de forma achatada
    std::complex<double>& at(int x, int y) {
        return dados[y * largura + x];
    }
    
    const std::complex<double>& at(int x, int y) const {
        return dados[y * largura + x];
    }
};