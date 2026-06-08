#pragma once

#include <vector>
#include <iostream>
#include <string>

struct Imagem {
    int largura;
    int altura;
    std::vector<double> dados;

    Imagem() : largura(0), altura(0) {}

    Imagem(int w, int h = 0) : largura(w){
        altura = (h == 0) ? w : h;
        dados.resize(w * h, 0.0);
    }

    Imagem(const std::vector<double>& vector, int w) : dados(vector), largura(w) {
        altura = dados.size() / largura;
    }

    inline double& at(int x, int y) {
        return dados[y * largura + x];
    }

    inline const double& at(int x, int y) const {
        return dados[y * largura + x];
    }
};

Imagem carregarImagem(const std::string& caminho);
void salvarImagem(const Imagem& img, const std::string& caminho);
Imagem aplicarZeroPadding(const Imagem& img, int novaLargura, int novaAltura, int deslocamentoX, int deslocamentoY);
int proximaPotenciaDe2(int maiorLado);