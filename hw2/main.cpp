#include <iostream>
#include <string>
#include "Picture.h"

#define x first
#define y second

typedef unsigned char uchar;

bool getLongDouble(char *str, double &value) {
    try {
        value = std::stod(str);
        return false;
    } catch (std::invalid_argument &e) {
        return true;
    }
}

int main(int argc, char **argv) {
    if (argc != 10 && argc != 9) {
        std::cerr
                << "Use linedrawer <input file> <output file> <line brightness> <line thickness> <x1> <y1> <x2> <y2> [gamma]"
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

    uchar lineBrightness;
    try {
        int intermediateValue = std::stoi(argv[3]);
        if (intermediateValue < 0 || intermediateValue > 255) {
            throw std::invalid_argument("");
        }
        lineBrightness = intermediateValue;
    } catch (std::invalid_argument &e) {
        std::cerr << "Incorrect args" << std::endl;
        in.close();
        out.close();
        remove(argv[2]);
        return 1;
    }
    double lineThickness, x1, y1, x2, y2, gamma = -1;
    if (getLongDouble(argv[4], lineThickness) || lineThickness < 0 ||
        getLongDouble(argv[5], x1) || getLongDouble(argv[6], y1) ||
        getLongDouble(argv[7], x2) || getLongDouble(argv[8], y2)) {
        std::cerr << "Incorrect args" << std::endl;
        in.close();
        out.close();
        remove(argv[2]);
        return 1;
    }
    if (argc == 10) {
        if (getLongDouble(argv[9], gamma)) {
            std::cerr << "Incorrect args" << std::endl;
            in.close();
            out.close();
            remove(argv[2]);
            return 1;
        }
    }
    Picture picture;
    if (picture.read(in)) {
        std::cerr << "Can't read input file" << std::endl;
        in.close();
        out.close();
        remove(argv[2]);
        return 1;
    }
    picture.drawLine(x1, y1, x2, y2, lineThickness, lineBrightness, {gamma});
    if (picture.write(out)) {
        std::cerr << "Can't write output file" << std::endl;
        in.close();
        out.close();
        remove(argv[2]);
        return 1;
    }
    in.close();
    out.close();
    return 0;
}