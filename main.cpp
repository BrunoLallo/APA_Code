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

RegistroBenchmark rodarBenchmarkEspSeparavel(const Imagem& img, const Imagem& kernel, int iteracoes) {
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

    int numLogs = 20;

    std::vector<RegistroBenchmark> logs(numLogs);

    std::cout << "Rodando aquecimento para estabilizar Cache e RAM...\n";
    rodarBenchmarkEspSeparavel(imgOriginal, Filtros::gerarMedia(3), 10);

    for (int i = 1; i <= numLogs; i++) {
        Imagem kernel = Filtros::gerarMedia(1 + i * 10);
        
        RegistroBenchmark log = rodarBenchmarkEspSeparavel(imgOriginal, kernel, 25);

        logs[i-1] = log;
    }

    exportarParaCSV(logs, relatoriosPath + "teste-Esp-Separavel.csv");

    return 0;
}