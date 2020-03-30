#ifndef HW2_GAMMACORRECTION_H
#define HW2_GAMMACORRECTION_H


class GammaCorrection {
private:
    long double gamma;

    int sRGB(int oldBrightness, int newBrightness, long double proportion);

    int exponential(int oldBrightness, int newBrightness, long double proportion);

public:
    GammaCorrection(long double gamma = -1.0);

    int getNewBrightness(int oldBrightness, int newBrightness, long double proportion);
};


#endif //HW2_GAMMACORRECTION_H
