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

    Imagem imgOriginal = carregarImagem(input + "test_image.png");

    int numLogs = 10;

    // Vetor mestre que irá armazenar os dados brutos de todas as iterações e de todos os kernels
    std::vector<RegistroBenchmark> logsGerais;

    std::cout << "[INFO] Rodando aquecimento para estabilizar Cache e RAM...\n";
    rodarBenchmarkEspSeparavel(imgOriginal, Filtros::gerarMedia(3), 10);

    for (int i = 1; i <= numLogs; i++) {
        int tamanhoKernel = 1 + i * 10;
        Imagem kernel = Filtros::gerarMedia(tamanhoKernel);
        
        int iteracoesAtuais = 50;
        if (tamanhoKernel > 31) iteracoesAtuais = 25;
        if (tamanhoKernel > 101) iteracoesAtuais = 10;

        std::cout << "[INFO] Testando Kernel " << tamanhoKernel << "x" << tamanhoKernel 
                  << " | Iteracoes: " << iteracoesAtuais << "...\n";
        
        std::vector<RegistroBenchmark> logsDoKernel = EXECUTAR_BENCHMARK(imgOriginal, kernel, iteracoesAtuais, convolucaoIngenua);

        logsGerais.insert(logsGerais.end(), logsDoKernel.begin(), logsDoKernel.end());
    }

    exportarParaCSV(logsGerais, relatoriosPath + "teste-Esp-Ingenua.csv");

    return 0;
}