#ifndef HW2_PICTURE_H
#define HW2_PICTURE_H


#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include "GammaCorrection.h"

class Picture {
public:
    typedef unsigned char uchar;

    bool read(std::ifstream &in);

    bool write(std::ofstream &out);

    bool readGradient(std::ifstream &in);

    Picture(const GammaCorrection &gammaCorrection);

    std::vector<uchar> data;

    size_t getWeight() const;

    size_t getHeight() const;

    const GammaCorrection &getGammaCorrection() const;

    Picture(std::vector<uchar> data, size_t weight, size_t height, const GammaCorrection &gammaCorrection);

private:
    size_t weight, height;
    GammaCorrection gammaCorrection;
};


#endif //HW2_PICTURE_H
