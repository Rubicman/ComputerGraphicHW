#include "GammaCorrection.h"
#include <cmath>

int GammaCorrection::sRGB(int oldBrightness, int newBrightness, double proportion) const {
    double a = oldBrightness / 255.0;
    a = a <= 0.04045 ? a / 12.92 : pow((a + 0.055) / 1.055, 2.4);
    double b = newBrightness / 255.0;
    b = b <= 0.04045 ? b / 12.92 : pow(((b + 0.055) / 1.055), 2.4);
    a = (1.0 - proportion) * a + proportion * b;
    a = a <= 0.0031308 ? 12.92 * a : pow(a, 0.416) * 1.055 - 0.055;
    return round(a * 255.0);
}

int GammaCorrection::exponential(int oldBrightness, int newBrightness, double proportion) const {
    double a = oldBrightness / 255.0;
    a = pow(a, gamma);
    double b = newBrightness / 255.0;
    b = pow(b, gamma);
    a = (1.0 - proportion) * a + proportion * b;
    a = pow(a, 1 / gamma);
    return round(a * 255.0);
}

int GammaCorrection::getNewBrightness(int oldBrightness, int newBrightness, double proportion) const{
    if (gamma == 0) {
        return sRGB(oldBrightness, newBrightness, proportion);
    } else {
        return exponential(oldBrightness, newBrightness, proportion);
    }
}


GammaCorrection::GammaCorrection(double gamma) : gamma(gamma) {}


int GammaCorrection::decode(double brightness) const{
    brightness /= 255.0;
    if (gamma == 0) {
        return round((brightness <= 0.0031308 ? 12.92 * brightness : pow(brightness, 0.416) * 1.055 - 0.055) * 255.0);
    } else {
        return round(pow(brightness, 1 / gamma) * 255.0);
    }
}

double GammaCorrection::encode(int brightness) const{
    double b = brightness / 255.0;
    if (gamma == 0) {
        return (b <= 0.04045 ? b / 12.92 : pow((b + 0.055) / 1.055, 2.4)) * 255.0;
    } else {
        return pow(b, gamma) * 255.0;
    }
};

