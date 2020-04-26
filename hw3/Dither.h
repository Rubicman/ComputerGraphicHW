#ifndef HW3_DITHER_H
#define HW3_DITHER_H

#include <vector>
#include "Picture.h"

class Dither {
private:
    typedef unsigned char uchar;
    const static int orderedMatrix[8][8];
    const static int halftoneMatrix[4][4];

    static Picture basicDither(Picture const &oldPicture, int bitness,
                               double (*ditherPixel)(double a, size_t i, size_t j, int bitness));

    static double orderedDP(double a, size_t i, size_t j, int bitness);

    static double randomDP(double a, size_t i, size_t j, int bitness);
    static double halftoneDP(double a, size_t i, size_t j, int bitness);

    static double roundPixel(uchar pixel, int bitness);

    static void
    addError(std::vector<uchar> &data, int weight, int height, int error, int sum, int value, int i, int j, int a,
             int b);

    static Picture errorDiffusionDither(Picture const &oldPicture, int bitness,
                                        void (*setError)(std::vector<uchar> &, int, int, int, int, int));

    static void floydSteinbergEDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j);

    static void jjnEDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j);

    static void sierra3EDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j);

    static void atkinsonEDD(std::vector<uchar> &data, int weight, int height, int error, int i, int j);

    static void
    setBrightness(GammaCorrection const &gc, std::vector<uchar> &data, int weight, int height, double value, int i,
                  int j);

public:
    static Picture round(Picture const &oldPicture, int bitness);

    static Picture ordered(Picture const &oldPicture, int bitness);

    static Picture random(Picture const &oldPicture, int bitness);

    static Picture floydSteinberg(Picture const &oldPicture, int bitness);

    static Picture jjn(Picture const &oldPicture, int bitness);

    static Picture sierra3(Picture const &oldPicture, int bitness);

    static Picture atkinson(Picture const &oldPicture, int bitness);

    static Picture halftone(Picture const &oldPicture, int bitness);
};


#endif //HW3_DITHER_H
