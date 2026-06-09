#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <complex>

#include <ImagemCore.h>
#include <Benchmark.h>
#include <ConvEspacial.h>
#include <ConvFrequencial.h>
#include <TransFourier.h>
#include <Filtros.h>

std::vector<RegistroBenchmark> rodarBenchmarkEspSeparavel(const Imagem& img, const Imagem& kernel, int iteracoes) {
    auto [v, h] = separarKernel(kernel);
    auto wrapperSeparavel = [&](const Imagem& i, const Imagem& k_dummy) {
        return convolucaoSeparavel(i, v, h);
    };

    return EXECUTAR_BENCHMARK(img, kernel, iteracoes, wrapperSeparavel);
}

int main() {
    std::string input = std::string(PASTA_IMAGENS);
    std::string output = std::string(PASTA_IMAGENS);

    std::string relatoriosPath = std::string(PASTA_RELATORIOS);

    Imagem imgOriginal = carregarImagem(input + "test_image_1024.png");

    int numLogs = 10;

    std::vector<RegistroBenchmark> logsEspIng;
    std::vector<RegistroBenchmark> logsEspSep;
    std::vector<RegistroBenchmark> logsFft2d;

    std::cout << "[INFO] Rodando aquecimento para estabilizar Cache e RAM...\n";
    rodarBenchmarkEspSeparavel(imgOriginal, Filtros::gerarMedia(3), 10);

    for (int i = 1; i <= numLogs; i++) {
        int tamanhoKernel = 1 + i * 5;
        Imagem kernel = Filtros::gerarMedia(tamanhoKernel);
        
        int iteracoesAtuais = 25;
        if (tamanhoKernel > 31) iteracoesAtuais = 10;
        if (tamanhoKernel > 101) iteracoesAtuais = 5;

        std::cout << "[INFO] Testando Kernel " << tamanhoKernel << "x" << tamanhoKernel 
                  << " | Iteracoes: " << iteracoesAtuais << "...\n";
        
        std::vector<RegistroBenchmark> logsDoKernel = EXECUTAR_BENCHMARK(imgOriginal, kernel, iteracoesAtuais, convolucaoIngenua);

        logsEspIng.insert(logsEspIng.end(), logsDoKernel.begin(), logsDoKernel.end());
    }

    exportarParaCSV(logsEspIng, relatoriosPath + "teste-Esp-Ingenua.csv");

    for (int i = 1; i <= numLogs; i++) {
        int tamanhoKernel = 1 + i * 5;
        Imagem kernel = Filtros::gerarMedia(tamanhoKernel);
        
        int iteracoesAtuais = 25;
        if (tamanhoKernel > 31) iteracoesAtuais = 10;
        if (tamanhoKernel > 101) iteracoesAtuais = 5;

        std::cout << "[INFO] Testando Kernel " << tamanhoKernel << "x" << tamanhoKernel 
                  << " | Iteracoes: " << iteracoesAtuais << "...\n";
        
        std::vector<RegistroBenchmark> logsDoKernel = rodarBenchmarkEspSeparavel(imgOriginal, kernel, iteracoesAtuais);

        logsEspSep.insert(logsEspSep.end(), logsDoKernel.begin(), logsDoKernel.end());
    }

    exportarParaCSV(logsEspSep, relatoriosPath + "teste-Esp-Separavel.csv");

    for (int i = 1; i <= numLogs; i++) {
        int tamanhoKernel = 1 + i * 5;
        Imagem kernel = Filtros::gerarMedia(tamanhoKernel);
        
        int iteracoesAtuais = 25;
        if (tamanhoKernel > 31) iteracoesAtuais = 10;
        if (tamanhoKernel > 101) iteracoesAtuais = 5;

        std::cout << "[INFO] Testando Kernel " << tamanhoKernel << "x" << tamanhoKernel 
                  << " | Iteracoes: " << iteracoesAtuais << "...\n";
        
        std::vector<RegistroBenchmark> logsDoKernel = EXECUTAR_BENCHMARK_FREQ(imgOriginal, kernel, iteracoesAtuais, fft2D);

        logsFft2d.insert(logsFft2d.end(), logsDoKernel.begin(), logsDoKernel.end());
    }

    exportarParaCSV(logsFft2d, relatoriosPath + "teste-Freq-FFT-2-Radix.csv");

    return 0;
}