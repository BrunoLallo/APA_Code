#pragma once

#include <ImagemCore.h>
#include <utility>

Imagem convolucaoIngenua(const Imagem& img, const Imagem& kernel);
Imagem convolucaoSeparavel(const Imagem& img, const std::vector<double>& verticalVec, const std::vector<double>& horizontalVec);
std::pair<std::vector<double>, std::vector<double>> separarKernel(const Imagem& kernel);