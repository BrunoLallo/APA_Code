#include <ImagemFreq.h>
#include <ConvFrequencial.h>

ImagemComplexa convFreq(const ImagemComplexa& img, const ImagemComplexa& kernel) {
    ImagemComplexa saida(img.largura, img.altura);

    for (int y = 0; y < img.altura; y++) {
        for (int x = 0; x < img.largura; x++) {
            saida.at(x, y) = img.at(x, y) * kernel.at(x, y);
        }
    }

    return saida;
}