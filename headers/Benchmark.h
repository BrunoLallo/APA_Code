#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <source_location>
#include <utility>
#include <cctype>
#include <malloc.h>
#include <ImagemCore.h>
#include <ImagemFreq.h>
#include <ConvEspacial.h>
#include <ConvFrequencial.h>
#include <TransFourier.h>

#define EXECUTAR_BENCHMARK(img, kernel, iteracoes, algoritmo) \
    rodarBenchmark(#algoritmo, img, kernel, iteracoes, algoritmo)

#define EXECUTAR_BENCHMARK_FREQ(img, kernel, iteracoes, algoritmo) \
    rodarBenchmarkFrequencia(#algoritmo, img, kernel, iteracoes, algoritmo)

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
            return std::stod(numeros);
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
    double picoMemoriaKB;
    int iteracoesDoKernel;
}; 

template <typename Func>
std::vector<RegistroBenchmark> rodarBenchmark(const std::string& nomeAlgoritmo, const Imagem& img, const Imagem& kernel, int iteracoes, Func algoritmo) {
    std::vector<RegistroBenchmark> logsDaExecucao;

    malloc_trim(0);
    double memoriaInicial = obterMemoriaRamMB();

    for (int i = 0; i < iteracoes; i++) {
        auto inicio = std::chrono::high_resolution_clock::now();

        Imagem resultado = algoritmo(img, kernel);

        double memoriaAtual = obterMemoriaRamMB() - memoriaInicial;

        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> tempoIteracao = fim - inicio;

        RegistroBenchmark log;
        log.algoritmo = nomeAlgoritmo;
        log.larguraImagem = img.largura;
        log.alturaImagem = img.altura;
        log.larguraKernel = kernel.largura;
        log.alturaKernel = kernel.altura;
        log.tempoMilissegundos = tempoIteracao.count(); // Regista o tempo APENAS desta iteração
        log.picoMemoriaKB = memoriaAtual;
        log.iteracoesDoKernel = iteracoes;

        logsDaExecucao.push_back(log);
    }

    return logsDaExecucao;
}

template <typename Func>
std::vector<RegistroBenchmark> rodarBenchmarkFrequencia(const std::string& nomeAlgoritmo, const Imagem& img, const Imagem& kernel, int iteracoes, Func transformada) {
    std::vector<RegistroBenchmark> logsDaExecucao;

    malloc_trim(0);
    double memoriaInicial = obterMemoriaRamMB();

    int larguraSegura = img.largura + kernel.largura - 1;
    int alturaSegura = img.altura + kernel.altura - 1;

    int tamanhoPot2 = proximaPotenciaDe2(larguraSegura);

    Imagem imgPad = aplicarZeroPadding(img, tamanhoPot2, tamanhoPot2, 0, 0);
    Imagem kernelPad = aplicarZeroPadding(kernel, tamanhoPot2, tamanhoPot2, 0, 0);
    
    for (int i = 0; i < iteracoes; i++) {
        auto inicio = std::chrono::high_resolution_clock::now();

        ImagemComplexa imgFFT = transformada(ImagemComplexa(imgPad), false);
        ImagemComplexa kernelFFT = transformada(ImagemComplexa(kernelPad), false);

        ImagemComplexa saida = convFreq(imgFFT, kernelFFT);

        ImagemComplexa inversa = transformada(saida, true);

        Imagem imgComPadding = calcularMagnitude(inversa);

        double memoriaAtual = obterMemoriaRamMB() - memoriaInicial;

        Imagem saidaFinal(img.largura, img.altura);
        int inicioX = kernel.largura / 2;
        int inicioY = kernel.altura / 2;

        for (int y = 0; y < img.altura; y++) {
            for (int x = 0; x < img.largura; x++) {
                saidaFinal.at(x, y) = imgComPadding.at(x + inicioX, y + inicioY);
            }
        }

        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> tempoIteracao = fim - inicio;

        RegistroBenchmark log;
        log.algoritmo = nomeAlgoritmo;
        log.larguraImagem = img.largura;
        log.alturaImagem = img.altura;
        log.larguraKernel = kernel.largura;
        log.alturaKernel = kernel.altura;
        log.tempoMilissegundos = tempoIteracao.count(); // Regista o tempo APENAS desta iteração
        log.picoMemoriaKB = memoriaAtual;
        log.iteracoesDoKernel = iteracoes;

        logsDaExecucao.push_back(log);
    }

    return logsDaExecucao;
}

void exportarParaCSV(const std::vector<RegistroBenchmark>& logs, const std::string& nomeArquivo);