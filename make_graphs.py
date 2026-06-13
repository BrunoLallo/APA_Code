import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

OUTPUT_PATH = "assets/"

ALGORITMOS = {
    "wrapperSeparavel": "Convolução Espacial Separável", 
    "convolucaoIngenua": "Convolução Espacial Ingênua",
    "fft2D": "Transformada Rápida de Fourier Bidimensional"}

def plotar_graficos_individuais(nome_arquivo):
    """
    Lê um arquivo CSV e gera dois gráficos separados (Tempo e Memória),
    salvando-os como imagens PNG.
    """
    df = pd.read_csv(nome_arquivo)
    
    # Extrai o nome do algoritmo (presumindo que seja o mesmo em todas as linhas)
    algoritmo = df['Algoritmo'].iloc[0]
    
    # ==========================================
    # Gráfico de Tempo
    # ==========================================
    plt.figure(figsize=(8, 5))
    plt.plot(df['LarguraKernel'], df['Tempo_ms'], marker='o', linestyle='-', color='b')
    plt.title(f'Tempo de Execução vs Tamanho do Kernel\n({ALGORITMOS[algoritmo]})')
    plt.xlabel('Tamanho do Kernel (Largura/Altura)')
    plt.ylabel('Tempo de Execução (ms)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    
    # Salva com o nome do algoritmo
    plt.savefig(f'{OUTPUT_PATH}{nome_arquivo.replace(".csv", "").split("/")[-1]}_tempo.png', dpi=300)
    plt.close()
    
    # ==========================================
    # Gráfico de Espaço (Memória)
    # ==========================================
    plt.figure(figsize=(8, 5))
    plt.plot(df['LarguraKernel'], df['Memoria_KB'], marker='s', linestyle='-', color='r')
    plt.title(f'Pico de Consumo de Memória vs Tamanho do Kernel\n({ALGORITMOS[algoritmo]})')
    plt.xlabel('Tamanho do Kernel (Largura/Altura)')
    plt.ylabel('Pico de Memória RAM (KB)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    
    plt.savefig(f'{OUTPUT_PATH}{nome_arquivo.replace(".csv", "").split("/")[-1]}_memoria.png', dpi=300)
    plt.close()

def plotar_graficos_combinados(lista_arquivos):
    """
    Recebe uma lista de arquivos CSV e cria gráficos comparativos 
    (Tempo e Memória) sobrepondo todos os algoritmos.
    """
    cores = ['blue', 'orange', 'green', 'red', 'purple']
    marcadores = ['o', 's', '^', 'D', 'v']

    # ==========================================
    # Gráfico Combinado de Tempo
    # ==========================================
    plt.figure(figsize=(10, 6))
    
    for idx, arquivo in enumerate(lista_arquivos):
        df = pd.read_csv(arquivo)
        algoritmo = df['Algoritmo'].iloc[0]
        
        plt.plot(df['LarguraKernel'], df['Tempo_ms'], 
                 marker=marcadores[idx % len(marcadores)], 
                 linestyle='-', 
                 color=cores[idx % len(cores)], 
                 label=ALGORITMOS[algoritmo])
        
    plt.title('Comparação de Tempo de Execução vs Tamanho do Kernel')
    plt.xlabel('Tamanho do Kernel (Largura/Altura)')
    plt.ylabel('Tempo de Execução (ms)')
    plt.legend(title="Algoritmos")
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig(f'{OUTPUT_PATH}comparacao_todos_tempo.png', dpi=300)
    plt.close()

    # ==========================================
    # Gráfico Combinado de Memória
    # ==========================================
    plt.figure(figsize=(10, 6))
    
    for idx, arquivo in enumerate(lista_arquivos):
        df = pd.read_csv(arquivo)
        algoritmo = df['Algoritmo'].iloc[0]
        
        plt.plot(df['LarguraKernel'], df['Memoria_KB'], 
                 marker=marcadores[idx % len(marcadores)], 
                 linestyle='-', 
                 color=cores[idx % len(cores)], 
                 label=ALGORITMOS[algoritmo])
        
    plt.title('Comparação de Consumo de Memória vs Tamanho do Kernel')
    plt.xlabel('Tamanho do Kernel (Largura/Altura)')
    plt.ylabel('Pico de Memória RAM (KB)')
    plt.legend(title="Algoritmos")
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig(f'{OUTPUT_PATH}comparacao_todos_memoria.png', dpi=300)
    plt.close()

def plotar_com_desvio_padrao(nome_arquivo):
    df_bruto = pd.read_csv(nome_arquivo)
    algoritmo = df_bruto['Algoritmo'].iloc[0]

    # Agrupa todos os dados que têm a mesma LarguraKernel
    # e calcula a Média e o Desvio Padrão (std) automaticamente
    df_agrupado = df_bruto.groupby('LarguraKernel')['Tempo_ms'].agg(['mean', 'std']).reset_index()

    plt.figure(figsize=(10, 6))
    
    # Plota a linha da média, e adiciona o yerr (Y-Error) para o desvio padrão
    plt.errorbar(df_agrupado['LarguraKernel'], df_agrupado['mean'], 
                 yerr=df_agrupado['std'], fmt='-o', color='b', capsize=5)

    plt.title(f'Tempo de Execução vs Tamanho do Kernel\ncom Desvio Padrão ({ALGORITMOS[algoritmo]})')
    plt.xlabel('Tamanho do Kernel')
    plt.ylabel('Tempo Médio (ms)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig(f'{OUTPUT_PATH}{nome_arquivo.replace(".csv", "").split("/")[-1]}_erro_estatistico.png', dpi=300)
    plt.close()

import pandas as pd
import matplotlib.pyplot as plt

arquivos = [
    'teste-Freq-FFT-2-Radix.csv',
    'teste-Freq-FFT-2-Radix-2.csv',
    'teste-Freq-FFT-2-Radix-3.csv',
    'teste-Freq-FFT-2-Radix-4.csv',
    'teste-Freq-FFT-2-Radix-5.csv'
]

def plotar_fft_multiplas_linhas(lista_arquivos, nome_saida="fft_linhas_separadas.png"):
    """
    Recebe uma lista de arquivos CSV, calcula a média de tempo para cada kernel 
    dentro do próprio arquivo e plota uma linha para cada execução independente.
    """
    plt.figure(figsize=(10, 6))
    
    # Paleta de cores e marcadores para diferenciar cada execução
    cores = ['blue', 'orange', 'green', 'red', 'purple']
    marcadores = ['o', 's', '^', 'D', 'v']

    for i, arquivo in enumerate(lista_arquivos):
        try:
            df = pd.read_csv(arquivo)
            algoritmo = df['Algoritmo'].iloc[0]
            
            # Agrupa os dados APENAS deste arquivo para obter a média
            df_agrupado = df.groupby('LarguraKernel')['Tempo_ms'].mean().reset_index()
            
            # Plota a linha deste arquivo (sem as barras de erro vertical)
            plt.plot(df_agrupado['LarguraKernel'], df_agrupado['Tempo_ms'], 
                     marker=marcadores[i % len(marcadores)], 
                     linestyle='-', 
                     color=cores[i % len(cores)],
                     label=f'{algoritmo} (Execução {i+1})')
        except Exception as e:
            print(f"Erro ao ler {arquivo}: {e}")

    plt.title('Tempo de Execução vs Tamanho do Kernel\n(Comparação de Execuções Independentes)')
    plt.xlabel('Tamanho do Kernel (Largura/Altura)')
    plt.ylabel('Tempo de Execução Médio (ms)')
    
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()
    
    # Salva o gráfico em alta resolução
    plt.savefig(nome_saida, dpi=300)
    print(f"Gráfico gerado com sucesso: {nome_saida}")
    plt.close()

if __name__ == "__main__":
    # Lista dos arquivos exportados pela sua suíte C++
    arquivos = [
        'relatorios/teste-Esp-Ingenua.csv', 
        'relatorios/teste-Esp-Separavel.csv',
        'relatorios/teste-Freq-FFT-2-Radix.csv',
        'relatorios/teste-Freq-FFT-2-Radix-2.csv',
        'relatorios/teste-Freq-FFT-2-Radix-3.csv',
        'relatorios/teste-Freq-FFT-2-Radix-4.csv',
        'relatorios/teste-Freq-FFT-2-Radix-5.csv'
    ]

    arquivos_comp_todos = [
        'relatorios/teste-Esp-Ingenua.csv', 
        'relatorios/teste-Esp-Separavel.csv',
        'relatorios/teste-Freq-FFT-2-Radix.csv'
        ]

    arquivos_fft = [
        'relatorios/teste-Freq-FFT-2-Radix.csv',
        'relatorios/teste-Freq-FFT-2-Radix-2.csv',
        'relatorios/teste-Freq-FFT-2-Radix-3.csv',
        'relatorios/teste-Freq-FFT-2-Radix-4.csv',
        'relatorios/teste-Freq-FFT-2-Radix-5.csv'
    ]

    plotar_fft_multiplas_linhas(arquivos_fft, nome_saida=f'{OUTPUT_PATH}fft_linhas_separadas.png')

    # Chama as funções para gerar todas as imagens de uma vez
    for csv_file in arquivos:
        plotar_graficos_individuais(csv_file)
        plotar_com_desvio_padrao(csv_file)
        
    plotar_graficos_combinados(arquivos_comp_todos)
    
    print("Gráficos gerados com sucesso no formato PNG de alta resolução (300 DPI)!")