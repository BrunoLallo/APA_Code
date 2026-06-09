#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <source_location>
#include <utility>
#include <cctype> // Necessário para std::isdigit
#include <ImagemCore.h>
#include <ImagemFreq.h>
#include <ConvEspacial.h>
#include <ConvFrequencial.h>
#include <TransFourier.h>

#define EXECUTAR_BENCHMARK(img, kernel, iteracoes, algoritmo) \
    rodarBenchmark(#algoritmo, img, kernel, iteracoes, algoritmo)

#define EXECUTAR_BENCHMARK_FREQ(img, kernel, iteracoes, algoritmo) \
    rodarBenchmarkFrequencia(#algoritmo, img, kernel, iteracoes, algoritmo)

// Função nativa do Linux para obter a memória RAM física (VmRSS) usada pelo processo em MB
inline double obterMemoriaRamMB() {
    std::ifstream arquivo("/proc/self/status");
    std::string linha;
    
    while (std::getline(arquivo, linha)) {
        if (linha.substr(0, 6) == "VmRSS:") {
            std::string numeros = "";
            for (char c : linha) {
                if (std::isdigit(c)) {
                    numeros += c;
                }
            }
            return std::stod(numeros) / 1024.0;
        }
    }
    return 0.0;
}

struct RegistroBenchmark {
    std::string algoritmo;
    int larguraImagem;
    int alturaImagem;
    int larguraKernel;
    int alturaKernel;
    double tempoMilissegundos;
    double picoMemoriaMB; // Novo campo de RAM
    int iteracoesDoKernel;
}; 

template <typename Func>
RegistroBenchmark rodarBenchmark(const std::string& nomeAlgoritmo, const Imagem& img, const Imagem& kernel, int iteracoes, Func algoritmo) {
    double tempoTotal = 0.0;
    double picoMemoriaDelta = 0.0;

    double memoriaInicial = obterMemoriaRamMB();

    for (int i = 0; i < iteracoes; i++) {
        auto inicio = std::chrono::high_resolution_clock::now();

        Imagem resultado = algoritmo(img, kernel);

        // Captura o pico de RAM logo após o processamento, enquanto 'resultado' e resíduos estão vivos
        double memoriaAtual = obterMemoriaRamMB() - memoriaInicial;
        if (memoriaAtual > picoMemoriaDelta) picoMemoriaDelta = memoriaAtual;

        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> tempoIteracao = fim - inicio;

        tempoTotal += tempoIteracao.count();
    }

    RegistroBenchmark log;
    log.algoritmo = nomeAlgoritmo;
    log.larguraImagem = img.largura;
    log.alturaImagem = img.altura;
    log.larguraKernel = kernel.largura;
    log.alturaKernel = kernel.altura;
    log.tempoMilissegundos = tempoTotal / iteracoes;
    log.picoMemoriaMB = picoMemoriaDelta; // Registra o consumo
    log.iteracoesDoKernel = iteracoes;

    return log;
}

template <typename Func>
RegistroBenchmark rodarBenchmarkFrequencia(const std::string& nomeAlgoritmo, const Imagem& img, const Imagem& kernel, int iteracoes, Func transformada) {
    double tempoTotal = 0.0;
    double picoMemoriaDelta = 0.0;

    double memoriaInicial = obterMemoriaRamMB();

    int larguraSegura = img.largura + kernel.largura - 1;
    int alturaSegura = img.altura + kernel.altura - 1;

    int tamanhoPot2 = proximaPotenciaDe2(larguraSegura);

    Imagem imgPad = aplicarZeroPadding(img, tamanhoPot2, tamanhoPot2, 0, 0);
    Imagem kernelPad = aplicarZeroPadding(kernel, tamanhoPot2, tamanhoPot2, 0, 0); // Correção: Passando 'kernel' ao invés de 'img'
    
    for (int i = 0; i < iteracoes; i++) {
        auto inicio = std::chrono::high_resolution_clock::now();

        ImagemComplexa imgFFT = transformada(ImagemComplexa(imgPad), false);
        ImagemComplexa kernelFFT = transformada(ImagemComplexa(kernelPad), false);

        ImagemComplexa saida = convFreq(imgFFT, kernelFFT);

        ImagemComplexa inversa = transformada(saida, true);

        Imagem imgComPadding = calcularMagnitude(inversa);

        // Ponto de maior exigência na RAM: Todas as matrizes complexas estão vivas na memória
        double memoriaAtual = obterMemoriaRamMB() - memoriaInicial;
        if (memoriaAtual > picoMemoriaDelta) picoMemoriaDelta = memoriaAtual;

        Imagem saidaFinal(img.largura, img.altura);
        int inicioX = kernel.largura / 2;
        int inicioY = kernel.altura / 2;

        for (int y = 0; y < img.altura; y++) {
            for (int x = 0; x < img.largura; x++) {
                saidaFinal.at(x, y) = imgComPadding.at(x + inicioX, y + inicioY);
            }
        }

        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> tempo = fim - inicio;

        tempoTotal += tempo.count();
    }

    RegistroBenchmark log;
    log.algoritmo = nomeAlgoritmo;
    log.larguraImagem = img.largura;
    log.alturaImagem = img.altura;
    log.larguraKernel = kernel.largura;
    log.alturaKernel = kernel.altura;
    log.tempoMilissegundos = tempoTotal / iteracoes;
    log.picoMemoriaMB = picoMemoriaDelta; // Registra o consumo
    log.iteracoesDoKernel = iteracoes;

    return log;
}

void exportarParaCSV(const std::vector<RegistroBenchmark>& logs, const std::string& nomeArquivo);