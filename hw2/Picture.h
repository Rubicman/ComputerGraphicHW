#ifndef HW2_PICTURE_H
#define HW2_PICTURE_H


#include <vector>
#include <fstream>
#include "GammaCorrection.h"
#include "Rectangle.h"

class Picture {
public:
    typedef unsigned char uchar;

    bool read(std::ifstream &in);
    bool write(std::ofstream &out);
    void drawLine(long double x1, long double y1, long double x2, long double y2, long double thickness, int brightness, GammaCorrection gammaCorrection);

private:
    std::vector<uchar> data;
    size_t weight, height;
};


#endif //HW2_PICTURE_H
