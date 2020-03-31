#ifndef HW2_PICTURE_H
#define HW2_PICTURE_H


#include <vector>
#include <fstream>
#include <algorithm>
#include "GammaCorrection.h"
#include "Rectangle.h"

class Picture {
public:
    typedef unsigned char uchar;

    bool read(std::ifstream &in);
    bool write(std::ofstream &out);
    void drawLine(double x1, double y1, double x2, double y2, double thickness, int brightness, GammaCorrection gammaCorrection);

private:
    std::vector<uchar> data;
    size_t weight, height;
};


#endif //HW2_PICTURE_H
