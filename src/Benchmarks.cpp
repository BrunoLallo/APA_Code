#include <Benchmark.h>

void exportarParaCSV(const std::vector<RegistroBenchmark>& logs, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir o arquivo: " << nomeArquivo << "\n";
        return;
    }

    arquivo << "Algoritmo,LarguraImagem,AlturaImagem,LarguraKernel,AlturaKernel,Tempo_ms,Iteracoes\n";

    for (const auto& log : logs) {
        arquivo << log.algoritmo << ","
                << log.larguraImagem << ","
                << log.alturaImagem << ","
                << log.larguraKernel << ","
                << log.alturaKernel << ","
                << log.tempoMilissegundos << ","
                << log.iteracoesDoKernel << "\n";
    }

    arquivo.close();
    std::cout << "[SUCESSO] Benchmark salvo em: " << nomeArquivo << "\n";
}