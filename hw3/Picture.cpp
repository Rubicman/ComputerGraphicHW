#include "Picture.h"
#include <string>
#include <utility>

bool Picture::read(std::ifstream &in) {
    std::string format;
    size_t maxValue;
    if (!(in >> format >> weight >> height >> maxValue) ||
        format != "P5" || maxValue != 255) {
        return true;
    }
    in.get();
    data.resize(weight * height);
    in.read((char *) data.data(), weight * height);
    return false;
}

bool Picture::write(std::ofstream &out) {
    out << "P5" << std::endl;
    out << weight << " " << height << std::endl;
    out << 255 << std::endl;
    out.write((char *) data.data(), data.size());
    return false;
}

bool Picture::readGradient(std::ifstream &in) {
    std::string format;
    size_t maxValue;
    if (!(in >> format >> weight >> height >> maxValue) ||
        format != "P5" || maxValue != 255) {
        return true;
    }
    in.get();
    data.resize(weight * height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            data[i * weight + j] = ((double) j) / height * 255.0;
        }
    }
    return false;
}

Picture::Picture(const GammaCorrection &gammaCorrection) : gammaCorrection(gammaCorrection) {}

size_t Picture::getWeight() const {
    return weight;
}

size_t Picture::getHeight() const {
    return height;
}

const GammaCorrection &Picture::getGammaCorrection() const {
    return gammaCorrection;
}

Picture::Picture(std::vector<uchar> data, size_t weight, size_t height, const GammaCorrection &gammaCorrection)
        : data(std::move(data)), weight(weight), height(height), gammaCorrection(gammaCorrection) {}

