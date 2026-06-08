#pragma once

#include <ImagemCore.h>
#include <ImagemFreq.h>

using Complexo = std::complex<double>;
using VetorComplexo = std::vector<std::complex<double>>;

void dft1D(std::vector<std::complex<double>>& sinal, bool inversa = false);
ImagemComplexa dft2D(ImagemComplexa img, bool inversa = false);
void fft1D_base(std::vector<std::complex<double>>& sinal, bool inversa = false);
void fft1D(std::vector<std::complex<double>>& sinal, bool inversa = false);
ImagemComplexa fft2D(ImagemComplexa img, bool inversa = false);
Imagem calcularMagnitude(const ImagemComplexa& img);
Imagem gerarEspectroVisual(const ImagemComplexa& imgFreq);
VetorComplexo extrairLinhaComplexa(const Imagem& img, int y);
VetorComplexo extrairColunaComplexa(const Imagem& img, int x);