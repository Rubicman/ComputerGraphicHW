#include <iostream>
#include <vector>
#include <fstream>
#include <string>


struct colored {
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    explicit colored(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0) : red(red), green(green),
                                                                                               blue(blue) {}

};

typedef unsigned char monochrome;

template<typename T>
std::vector<T> read(std::ifstream &in, size_t size);

template<>
std::vector<colored> read(std::ifstream &in, size_t size) {
    std::vector<colored> data(size);
    in.read((char *) data.data(), size * 3);
    return data;
}

template<>
std::vector<monochrome> read(std::ifstream &in, size_t size) {
    std::vector<monochrome> data(size);
    in.read((char *) data.data(), size);
    return data;
}

template<typename T>
void write(std::ofstream &out, std::vector<T> const &data);

template<>
void write(std::ofstream &out, std::vector<colored> const &data) {
    out << 255 << std::endl;
    out.write((char *) data.data(), data.size() * 3);
}

template<>
void write(std::ofstream &out, std::vector<monochrome> const &data) {
    out << 255 << std::endl;
    out.write((char *) data.data(), data.size());
}

template<typename T>
T negate(T pixel);

template<>
colored negate(colored pixel) {
    return colored(255 - pixel.red, 255 - pixel.green, 255 - pixel.blue);
}

template<>
monochrome negate(monochrome pixel) {
    return 255 - pixel;
}

template<typename T>
void inversionConverse(std::vector<T> &data, size_t &weight, size_t &height) {
    for (size_t i = 0; i < weight * height; i++) {
        data[i] = negate<T>(data[i]);
    }
}

template<typename T>
void verticalConverse(std::vector<T> &data, size_t &weight, size_t &height) {
    for (size_t i = 0; i < weight; i++) {
        for (size_t j = 0; j < height / 2; j++) {
            std::swap(data[j * weight + i], data[(height - j - 1) * weight + i]);
        }
    }
}

template<typename T>
void horizontalConverse(std::vector<T> &data, size_t &weight, size_t &height) {
    for (size_t i = 0; i < weight / 2; i++) {
        for (size_t j = 0; j < height; j++) {
            std::swap(data[j * weight + i], data[j * weight + (weight - i - 1)]);
        }
    }
}

template<typename T>
void clockwiseConverse(std::vector<T> &data, size_t &weight, size_t &height) {
    auto newData = std::vector<T>(weight * height);
    for (size_t i = 0; i < weight; i++) {
        for (size_t j = 0; j < height; j++) {
            newData[i * height + (height - j - 1)] = data[j * weight + i];
        }
    }
    std::swap(data, newData);
    std::swap(weight, height);
}

template<typename T>
void counterClockwiseConverse(std::vector<T> &data, size_t &weight, size_t &height) {
    auto newData = std::vector<T>(weight * height);
    for (size_t i = 0; i < weight; i++) {
        for (size_t j = 0; j < height; j++) {
            newData[(weight - i - 1) * height + j] = data[j * weight + i];
        }
    }
    std::swap(data, newData);
    std::swap(weight, height);
}

template<typename T>
bool converse(std::ifstream &in, std::ofstream &out, size_t weight, size_t height, std::string const &conversion) {
    auto data = read<T>(in, weight * height);
    if (conversion == "0") {
        inversionConverse(data, weight, height);
    } else if (conversion == "1") {
        horizontalConverse(data, weight, height);
    } else if (conversion == "2") {
        verticalConverse(data, weight, height);
    } else if (conversion == "3") {
        clockwiseConverse(data, weight, height);
    } else if (conversion == "4") {
        counterClockwiseConverse(data, weight, height);
    } else {
        return false;
    }
    out << weight << " " << height << std::endl;
    write<T>(out, data);
    return true;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Use 'converter.exe <input file> <output file> <conversion>'" << std::endl;
        return 1;
    }
    std::ifstream in(argv[1], std::ifstream::binary);
    std::ofstream out(argv[2], std::ifstream::binary);
    if (!in.is_open()) {
        std::cerr << "Can't open input file";
        in.close();
        out.close();
        return 1;
    }
    if (!out.is_open()) {
        std::cerr << "Can't open output file";
        in.close();
        out.close();
        return 1;
    }
    std::string format;
    size_t weight, height, maxValue;
    if (!(in >> format >> weight >> height >> maxValue)) {
        std::cerr << "Can't read from input file";
        in.close();
        out.close();
        return 1;
    }
    in.get();
    out << format << std::endl;
    bool result;
    try {
        if (format == "P5" && maxValue == 255) {
            result = converse<monochrome>(in, out, weight, height, argv[3]);
        } else if (format == "P6" && maxValue == 255) {
            result = converse<colored>(in, out, weight, height, argv[3]);
        } else {
            std::cerr << "Unknown format of input file" << std::endl;
            in.close();
            out.close();
            return 1;
        }
    } catch (std::bad_alloc &e) {
        std::cerr << "Not enough memory" << std::endl;
        in.close();
        out.close();
        return 1;
    }
    if (!result) {
        std::cerr << "Unknown conversion" << std::endl;
        in.close();
        out.close();
        return 1;
    }
    in.close();
    out.close();
    return 0;
}