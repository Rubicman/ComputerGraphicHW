#include <iostream>
#include <fstream>
#include <string>
#include "Picture.h"
#include "Dither.h"

bool getInt(char *str, int &value) {
    try {
        value = std::stoi(str);
        return false;
    } catch (std::invalid_argument &e) {
        return true;
    }
}

bool getDouble(char *str, double &value) {
    try {
        value = std::stod(str);
        return false;
    } catch (std::invalid_argument &e) {
        return true;
    }
}

int main(int argc, char **argv) {
    if (argc != 7) {
        std::cerr
                << "Use dither <input file> <output file> <gradient> <dithering> <bitness> <gamma>"
                << std::endl;
        return 1;
    }

    std::ifstream in(argv[1], std::ios_base::binary);
    if (!in.is_open()) {
        std::cerr << "Can't open input file" << std::endl;
        return 1;
    }

    std::ofstream out(argv[2], std::ios_base::binary);
    if (!out.is_open()) {
        std::cerr << "Can't open output file" << std::endl;
        in.close();
        return 1;
    }
    int gradient, dithering, bitness;
    double gamma;
    if (getInt(argv[3], gradient) || !(gradient == 0 || gradient == 1)
        || getInt(argv[4], dithering) || !(dithering >= 0 && dithering <= 7)
        || getInt(argv[5], bitness) || !(bitness >= 1 && bitness <= 8)
        || getDouble(argv[6], gamma) || gamma < 0) {
        std::cerr << "Incorrect args" << std::endl;
        in.close();
        out.close();
        remove(argv[2]);
        return 1;
    }
    Picture picture((GammaCorrection(gamma)));
    try {
        if (gradient) {
            picture.readGradient(in);
        } else {
            picture.read(in);
        }
    } catch (std::bad_alloc &e) {
        std::cerr << "Could not alloc memory" << std::endl;
        in.close();
        out.close();
        remove(argv[2]);
        return 1;
    }
    if (dithering == 1) {
        picture = Dither::ordered(picture, bitness);
    }
    if (dithering == 2) {
        picture = Dither::random(picture, bitness);
    }
    if (dithering == 3) {
        picture = Dither::floydSteinberg(picture, bitness);
    }
    if (dithering == 4) {
        picture = Dither::jjn(picture, bitness);
    }
    if (dithering == 5) {
        picture = Dither::sierra3(picture, bitness);
    }
    if (dithering == 6) {
        picture = Dither::atkinson(picture, bitness);
    }
    if (dithering == 7) {
        picture = Dither::halftone(picture, bitness);
    }
    picture = Dither::round(picture, bitness);
    picture.write(out);
    in.close();
    out.close();
    return 0;
}