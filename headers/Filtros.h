#pragma once

#include <ImagemCore.h>
#include <cmath>

namespace Filtros {
    inline const Imagem SobelX({
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1}, 3);

    inline const Imagem SobelY({
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1}, 3);

    inline Imagem gerarMedia(int k) {
        double valor = 1.0 / (k * k);
        std::vector<double> pesos(k * k, valor);
        return Imagem(pesos, k);
    }

    inline Imagem gerarGaussiano(int k, double sigma = 1.0) {
        std::vector<double> pesos(k * k);
        double soma = 0.0;
        int centro = k / 2;

        for (int y = 0; y < k; y++) {
            for (int x = 0; x < k; x++) {
                int dx = x - centro;
                int dy = y - centro;

                double valor = std::exp(-(dx * dx + dy * dy) / (2 * sigma * sigma));
                pesos[y * k + x] = valor;
                soma += valor;
            }
        }

        for (int i = 0; i < k * k; i++) {
            pesos[i] /= soma;
        }

        return Imagem(pesos, k);
    }
}