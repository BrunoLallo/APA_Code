#include <TransFourier.h>
#include <ImagemFreq.h>

void dft1D(std::vector<std::complex<double>>& sinal, bool inversa) {
    int N = sinal.size();
    std::vector<std::complex<double>> saida(N, 0.0);

    double sinalAngulo = inversa ? 1.0 : -1.0;
    
    for (int k = 0; k < N; k++) {
        std::complex<double> soma(0.0, 0.0);

        for (int n = 0; n < N; n++) {
            double theta = sinalAngulo * 2.0 * M_PI * k * n / N;
            std::complex<double> expoente = std::polar(1.0, theta);
            soma += sinal[n] * expoente;
        }

        if (inversa) {
            soma /= static_cast<double>(N);
        }

        saida[k] = soma;
    }

    sinal = saida;
}

ImagemComplexa dft2D(ImagemComplexa img, bool inversa) {
    for (int y = 0; y < img.altura; y++) {
        std::vector<std::complex<double>> linha(img.largura);

        for (int x = 0; x < img.largura; x++) {
            linha[x] = img.at(x, y);
        }

        dft1D(linha, inversa);

        for (int x = 0; x < img.largura; x++) {
            img.at(x, y) = linha[x];
        }
    }

    for (int x = 0; x < img.largura; x++) {
        std::vector<std::complex<double>> coluna(img.altura);

        for (int y = 0; y < img.altura; y++) {
            coluna[y] = img.at(x, y);
        }

        dft1D(coluna, inversa);

        for (int y = 0; y < img.altura; y++) {
            img.at(x, y) = coluna[y];
        }
    }

    return img;
}

void fft1D_base(std::vector<std::complex<double>>& sinal, bool inversa) {
    int N = sinal.size();

    if (N <= 1) return;

    std::vector<std::complex<double>> pares(N / 2);
    std::vector<std::complex<double>> impares(N / 2);

    for (int i = 0; i < N / 2; i++) {
        pares[i] = sinal[i * 2];
        impares[i] = sinal[i * 2 + 1];
    }

    fft1D_base(pares, inversa);
    fft1D_base(impares, inversa);

    double sinal_angulo = inversa ? 1.0 : -1.0;

    for (int k = 0; k < N / 2; k++) {
        double theta = sinal_angulo * 2.0 * M_PI * k / N;
        std::complex<double> twiddle = std::polar(1.0, theta) * impares[k];

        sinal[k] = pares[k] + twiddle;
        sinal[k + N / 2] = pares[k] - twiddle;
    } 
}

void fft1D(std::vector<std::complex<double>>& sinal, bool inversa) {
    fft1D_base(sinal, inversa);

    if (inversa) {
        int N = sinal.size();
        for (int i = 0; i < N; i++) {
            sinal[i] /= static_cast<double>(N);
        }
    }
}

ImagemComplexa fft2D(ImagemComplexa img, bool inversa) {
    for (int y = 0; y < img.altura; y++) {
        std::vector<std::complex<double>> linha(img.largura);
        for (int x = 0; x < img.largura; x++) {
            linha[x] = img.at(x, y);
        }

        fft1D(linha, inversa);

        for (int x = 0; x < img.largura; x++) {
            img.at(x, y) = linha[x]; 
        }
    }

    for (int x = 0; x < img.largura; x++) {
        std::vector<std::complex<double>> coluna(img.altura);
        for (int y = 0; y < img.altura; y++) {
            coluna[y] = img.at(x, y);
        }

        fft1D(coluna, inversa);

        for (int y = 0; y < img.altura; y++) {
            img.at(x, y) = coluna[y];
        }
    }

    return img;
}

Imagem calcularMagnitude(const ImagemComplexa& img) {
    Imagem saida(img.largura, img.altura);

    for (int y = 0; y < img.altura; y++) {
        for (int x = 0; x < img.largura; x++) {
            saida.at(x, y) = std::abs(img.at(x, y));
        }
    }

    return saida;
}

Imagem gerarEspectroVisual(const ImagemComplexa& img) {
    Imagem espectro = calcularMagnitude(img);

    int w = img.largura;
    int h = img.altura;

    double maxMag = 0.0;

    for (int i = 0; i < w * h; i++) {
        espectro.dados[i] = std::log(1.0 + espectro.dados[i]);
        if (espectro.dados[i] > maxMag) {
            maxMag = espectro.dados[i];
        }
    }

    if (maxMag > 0.0) {
        for (int i = 0; i < w * h; i++) {
            espectro.dados[i] = (espectro.dados[i] / maxMag);
        }
    }

    Imagem espectroShifted(w, h);

    int cx = w / 2;
    int cy = h / 2;

    for (int y = 0; y < cy; y++) {
        for (int x = 0; x < cx; x++) {
            espectroShifted.at(x + cx, y + cy) = espectro.at(x, y);
            espectroShifted.at(x, y) = espectro.at(x + cx, y + cy);

            espectroShifted.at(x, y + cy) = espectro.at(x + cx, y);
            espectroShifted.at(x + cx, y) = espectro.at(x, y + cy);
        }
    }

    return espectroShifted;
}

VetorComplexo extrairLinhaComplexa(const Imagem& img, int y) {
    VetorComplexo linha(img.largura);
    for (int x = 0; x < img.largura; x++) {
        linha[x] = Complexo(img.at(x, y), 0.0);
    }

    return linha;
}

VetorComplexo extrairColunaComplexa(const Imagem& img, int x) {
    VetorComplexo coluna(img.altura);
    for (int y = 0; y < img.altura; y++) {
        coluna[y] = Complexo(img.at(x, y), 0.0);
    }

    return coluna;
}