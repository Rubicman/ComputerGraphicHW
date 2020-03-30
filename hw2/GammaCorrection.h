#ifndef HW2_GAMMACORRECTION_H
#define HW2_GAMMACORRECTION_H


class GammaCorrection {
private:
    double gamma;

    int sRGB(int oldBrightness, int newBrightness, double proportion);

    int exponential(int oldBrightness, int newBrightness, double proportion);

public:
    GammaCorrection(double gamma = -1.0);

    int getNewBrightness(int oldBrightness, int newBrightness, double proportion);
};


#endif //HW2_GAMMACORRECTION_H
