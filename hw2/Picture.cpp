#include "Picture.h"

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

void Picture::drawLine(double x1, double y1, double x2, double y2, double thickness, int brightness, GammaCorrection gammaCorrection) {
    Rectangle rectangle = Rectangle(x1, y1, x2, y2, thickness);
    size_t iMin = std::max(0ll, (long long) (std::min(y1, y2) - thickness / 2 - 2));
    size_t jMin = std::max(0ll, (long long) (std::min(x1, x2) - thickness / 2 - 2));
    size_t iMax = std::min(height, (size_t) (std::max(y1, y2) + thickness / 2 + 2));
    size_t jMax = std::min(weight, (size_t) (std::max(x1, x2) + thickness / 2 + 2));
    for (size_t i = iMin; i < iMax; i++) {
        for (size_t j = jMin; j < jMax; j++) {
            double s = rectangle.intersectionArea({(double) j, (double) i});
            data[i * weight + j] = gammaCorrection.getNewBrightness(data[i * weight + j], brightness, s);
        }
    }
}

