#ifndef HW2_GAMMACORRECTION_H
#define HW2_GAMMACORRECTION_H


class GammaCorrection {
private:
    double gamma;

    int sRGB(int oldBrightness, int newBrightness, double proportion) const;

    int exponential(int oldBrightness, int newBrightness, double proportion) const;

public:
    GammaCorrection(double gamma = 0);

    int getNewBrightness(int oldBrightness, int newBrightness, double proportion) const;

    double encode(int brightness) const;

    int decode(double brightness) const;
};


#endif //HW2_GAMMACORRECTION_H
