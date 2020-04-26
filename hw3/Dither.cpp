#include <cmath>
#include "Dither.h"
#include "Picture.h"

constexpr const int Dither::orderedMatrix[8][8] = {{0,  48, 12, 60, 3,  51, 15, 63},
                                                   {32, 16, 44, 28, 35, 19, 47, 31},
                                                   {8,  56, 4,  52, 11, 59, 7,  55},
                                                   {40, 24, 36, 20, 43, 27, 39, 23},
                                                   {2,  50, 14, 62, 1,  49, 13, 61},
                                                   {34, 18, 46, 30, 33, 17, 45, 29},
                                                   {10, 58, 6,  54, 9,  57, 5,  53},
                                                   {42, 26, 38, 22, 41, 25, 37, 21}};

constexpr const int Dither::halftoneMatrix[4][4] = {{6,  12, 10, 3},
                                                    {11, 15, 13, 7},
                                                    {9,  14, 5,  1},
                                                    {4,  8,  2,  0}};

Picture Dither::ordered(Picture const &oldPicture, int bitness) {
    return basicDither(oldPicture, bitness, orderedDP);
}

Picture Dither::round(Picture const &oldPicture, int bitness) {
    std::vector<uchar> newData(oldPicture.data);
    for (uchar &a:newData) {
        a = roundPixel(a, bitness);
    }
    return Picture(newData, oldPicture.getWeight(), oldPicture.getHeight(), oldPicture.getGammaCorrection());
}

Picture
Dither::basicDither(Picture const &oldPicture, int bitness, double (*ditherPixel)(double, size_t, size_t, int)) {
    std::vector<uchar> newData(oldPicture.data);
    GammaCorrection const &gc = oldPicture.getGammaCorrection();
    int weight = oldPicture.getWeight();
    int height = oldPicture.getHeight();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            double a = gc.encode(newData[i * weight + j]);
            a = ditherPixel(a, i, j, bitness);
            a = std::min(255.0, std::max(0.0, a));
            a = gc.decode(a);
            newData[i * weight + j] = a;
        }
    }
    return Picture(newData, weight, height, gc);
}

double Dither::orderedDP(double a, size_t i, size_t j, int bitness) {
    double r = 255.0 / (1 << bitness);
    return (a + r * ((orderedMatrix[i % 8][j % 8] + 1) / 64.0 - 0.5));
}

Picture Dither::random(Picture const &oldPicture, int bitness) {
    return basicDither(oldPicture, bitness, randomDP);
}

double Dither::randomDP(double a, size_t i, size_t j, int bitness) {
    double r = 255.0 / (1 << bitness);
    return a + r * (((double) rand() + 1) / RAND_MAX - 0.5);
}

double Dither::roundPixel(Dither::uchar pixel, int bitness) {
    if (bitness >= 4) {
        pixel = ((pixel >> (8 - bitness)) << (8 - bitness));

    }
    if (bitness == 1) {
        pixel = ((uchar) -1) * (pixel >> 7);

    }
    if (bitness == 2) {
        pixel = pixel >> 6;
        pixel = (pixel << 6) + (pixel << 4) + (pixel << 2) + pixel;

    }
    if (bitness == 3) {
        pixel = pixel >> 5;
        pixel = (pixel << 5) + (pixel << 2) + (pixel >> 1);
    }
    return pixel;
}

Picture Dither::floydSteinberg(Picture const &oldPicture, int bitness) {
    return errorDiffusionDither(oldPicture, bitness, floydSteinbergEDD);
}

Picture Dither::jjn(Picture const &oldPicture, int bitness) {
    return errorDiffusionDither(oldPicture, bitness, jjnEDD);
}

void
Dither::addError(std::vector<uchar> &data, int weight, int height, int error, int sum, int value, int i, int j, int a,
                 int b) {
    if (i + a >= 0 && j + b >= 0 && i + a < height && j + b < weight) {
        data[(i + a) * weight + j + b] += error * value / sum;
    }
}

Picture Dither::sierra3(Picture const &oldPicture, int bitness) {
    return errorDiffusionDither(oldPicture, bitness, sierra3EDD);
}

Picture
Dither::errorDiffusionDither(Picture const &oldPicture, int bitness,
                             void (*setError)(std::vector<Dither::uchar> &, int, int, int, int, int)) {
    std::vector<uchar> newData(oldPicture.data);
    GammaCorrection const &gc = oldPicture.getGammaCorrection();
    int weight = oldPicture.getWeight();
    int height = oldPicture.getHeight();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            uchar a = newData[i * weight + j];
            uchar b = roundPixel(a, bitness);
            int error = ((int) a) - b;
            setError(newData, weight, height, error, i, j);
            newData[i * weight + j] = b;
        }
    }
    return Picture(newData, weight, height, gc);
}

void Dither::floydSteinbergEDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j) {
    addError(data, weight, height, error, 16, 7, i, j, 0, 1);
    addError(data, weight, height, error, 16, 3, i, j, 1, -1);
    addError(data, weight, height, error, 16, 5, i, j, 1, 0);
    addError(data, weight, height, error, 16, 1, i, j, 1, 1);
}

void Dither::jjnEDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j) {
    addError(data, weight, height, error, 48, 7, i, j, 0, 1);
    addError(data, weight, height, error, 48, 5, i, j, 0, 2);

    addError(data, weight, height, error, 48, 3, i, j, 1, -2);
    addError(data, weight, height, error, 48, 5, i, j, 1, -1);
    addError(data, weight, height, error, 48, 7, i, j, 1, 0);
    addError(data, weight, height, error, 48, 5, i, j, 1, 1);
    addError(data, weight, height, error, 48, 3, i, j, 1, 2);

    addError(data, weight, height, error, 48, 1, i, j, 2, -2);
    addError(data, weight, height, error, 48, 3, i, j, 2, -1);
    addError(data, weight, height, error, 48, 5, i, j, 2, 0);
    addError(data, weight, height, error, 48, 3, i, j, 2, 1);
    addError(data, weight, height, error, 48, 1, i, j, 2, 2);
}

void Dither::sierra3EDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j) {
    addError(data, weight, height, error, 32, 5, i, j, 0, 1);
    addError(data, weight, height, error, 32, 3, i, j, 0, 2);

    addError(data, weight, height, error, 32, 2, i, j, 1, -2);
    addError(data, weight, height, error, 32, 4, i, j, 1, -1);
    addError(data, weight, height, error, 32, 5, i, j, 1, 0);
    addError(data, weight, height, error, 32, 4, i, j, 1, 1);
    addError(data, weight, height, error, 32, 2, i, j, 1, 2);

    addError(data, weight, height, error, 32, 2, i, j, 2, -1);
    addError(data, weight, height, error, 32, 3, i, j, 2, 0);
    addError(data, weight, height, error, 32, 2, i, j, 2, 1);
}

void Dither::atkinsonEDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j) {
    addError(data, weight, height, error, 8, 1, i, j, 0, 1);
    addError(data, weight, height, error, 8, 1, i, j, 0, 2);

    addError(data, weight, height, error, 8, 1, i, j, 1, -1);
    addError(data, weight, height, error, 8, 1, i, j, 1, 0);
    addError(data, weight, height, error, 8, 1, i, j, 1, 1);

    addError(data, weight, height, error, 8, 1, i, j, 2, 0);
}

Picture Dither::atkinson(Picture const &oldPicture, int bitness) {
    return errorDiffusionDither(oldPicture, bitness, atkinsonEDD);
}

Picture Dither::halftone(Picture const &oldPicture, int bitness) {
    return basicDither(oldPicture, bitness, halftoneDP);
}

void
Dither::setBrightness(GammaCorrection const &gc, std::vector<uchar> &data, int weight, int height, double value, int i,
                      int j) {
    if (i < height && j < weight) {
        data[i * weight + j] = value * 255;
    }
}

double Dither::halftoneDP(double a, size_t i, size_t j, int bitness) {
    double r = 255.0 / (1 << bitness);
    return (a + r * ((halftoneMatrix[i % 4][j % 4] + 1) / 16.0 - 0.5));
}

