#include <Benchmark.h>

void exportarParaCSV(const std::vector<RegistroBenchmark>& logs, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir o arquivo: " << nomeArquivo << "\n";
        return;
    }

    // Adicionado o cabeçalho 'Memoria_MB'
    arquivo << "Algoritmo,LarguraImagem,AlturaImagem,LarguraKernel,AlturaKernel,Tempo_ms,Memoria_MB,Iteracoes\n";

    for (const auto& log : logs) {
        arquivo << log.algoritmo << ","
                << log.larguraImagem << ","
                << log.alturaImagem << ","
                << log.larguraKernel << ","
                << log.alturaKernel << ","
                << log.tempoMilissegundos << ","
                << log.picoMemoriaMB << "," // Escrita da nova variável no CSV
                << log.iteracoesDoKernel << "\n";
    }

    arquivo.close();
    std::cout << "[SUCESSO] Benchmark salvo em: " << nomeArquivo << "\n";
}