#include "GammaCorrection.h"
#include <cmath>

int GammaCorrection::sRGB(int oldBrightness, int newBrightness, double proportion) {
    double a = oldBrightness / 255.0;
    a = a <= 0.04045 ? a / 12.92 : powl((a + 0.055) / 1.055, 2.4);
    double b = newBrightness / 255.0;
    b = b <= 0.04045 ? b / 12.92 : powl(((b + 0.055) / 1.055), 2.4);
    a = (1.0 - proportion) * a + proportion * b;
    a = a <= 0.0031308 ? 12.92 * a : powl(a, 0.416) * 1.055 - 0.055;
    return roundl(a * 255.0);
}

int GammaCorrection::exponential(int oldBrightness, int newBrightness, double proportion) {
    double a = oldBrightness / 255.0;
    a = powl(a, gamma);
    double b = newBrightness / 255.0;
    b = powl(b, gamma);
    a = (1.0 - proportion) * a + proportion * b;
    a = powl(a, 1 / gamma);
    return roundl(a * 255.0);
}

int GammaCorrection::getNewBrightness(int oldBrightness, int newBrightness, double proportion) {
    if (gamma == -1.0) {
        return sRGB(oldBrightness, newBrightness, proportion);
    } else {
        return exponential(oldBrightness, newBrightness, proportion);
    }
}

GammaCorrection::GammaCorrection(double gamma)  : gamma(gamma) {};

