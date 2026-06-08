#include <ConvEspacial.h>

Imagem convolucaoIngenua(const Imagem& img, const Imagem& kernel) {
    Imagem saida(img.largura, img.altura);
    int K = kernel.largura;
    int pad = K / 2;

    for (int y = 0; y < img.altura; y++) {
        for (int x = 0; x < img.largura; x++) {

            double soma = 0.0;

            for (int ky = 0; ky < K; ky++) {
                for (int kx = 0; kx < K; kx++) {
                    int img_x = x + kx - pad;
                    int img_y = y + ky - pad;

                    if (img_x >= 0 && img_x < img.largura && img_y >= 0 && img_y < img.altura) {
                        soma += img.at(img_x, img_y) * kernel.at(kx, ky);
                    }
                }
            }

            saida.at(x, y) = soma;
        }
    }

    return saida;
}

Imagem convolucaoSeparavel(const Imagem& img, const std::vector<double>& verticalVec, const std::vector<double>& horizontalVec) {
    Imagem imgBuffer(img.largura, img.altura);
    Imagem saida(img.largura, img.altura);

    int verticalPad = verticalVec.size() / 2;
    int horizontalPad = horizontalVec.size() / 2;

    for (int y = 0; y < imgBuffer.altura; y++) {
        for (int x = 0; x < imgBuffer.largura; x++) {
            double soma = 0.0;

            for (int k = 0; k < horizontalVec.size(); k++) {
                int img_x = x + k - horizontalPad;

                if (img_x >= 0 && img_x < imgBuffer.largura) {
                    soma += img.at(img_x, y) * horizontalVec[k];
                }
            }

            imgBuffer.at(x, y) = soma;
        }
    }

    for (int y = 0; y < imgBuffer.altura; y++) {
        for (int x = 0; x < imgBuffer.largura; x++) {
            double soma = 0.0;

            for (int k = 0; k < verticalVec.size(); k++) {
                int img_y = y + k - verticalPad;

                if (img_y >= 0 && img_y < imgBuffer.altura) {
                    soma += imgBuffer.at(x, img_y) * verticalVec[k];
                }
            }

            saida.at(x, y) = soma;
        }
    }

    return saida;
}

std::pair<std::vector<double>, std::vector<double>> separarKernel(const Imagem& kernel) {   
    int linePivot = 0;
    int columnPivot = 0;
    double max = -1.0;

    for (int i = 0; i < kernel.dados.size(); i++) {
        if (kernel.dados[i] > max) {
            max = kernel.dados[i];
            linePivot = i / kernel.largura;
            columnPivot = i % kernel.largura; 
        }
    }

    std::vector<double> verticalVec(kernel.altura);
    std::vector<double> horizontalVec(kernel.largura);

    for (int i = 0; i < kernel.altura; i++) {
        verticalVec[i] = kernel.dados[i * kernel.largura + columnPivot] / max;
    }

    for (int i = 0; i < kernel.largura; i++) {
        horizontalVec[i] = kernel.dados[linePivot * kernel.largura + i];
    }

    bool isRank1 = true;
    double epsilon = 1e-6;

    for (int y = 0; y < kernel.altura; y++) {
        for (int x = 0; x < kernel.largura; x++) {
            double reconstructed = verticalVec[y] * horizontalVec[x];
            double original = kernel.dados[y * kernel.largura + x];

            if (std::abs(reconstructed - original) > epsilon) {
                isRank1 = false;
                break;
            }
        }
        if (!isRank1) break;
    }

    if (!isRank1) {
        return {{}, {}};
    }

    return {verticalVec, horizontalVec};    
}