#include "Pixel.h"

#define x first
#define y second

Line::Point Pixel::getRightUp() const {
    return borderPoints[1];
}

Line::Point Pixel::getRightDown() const {
    return borderPoints[2];
}

Line::Point Pixel::getLeftUp() const {
    return borderPoints[0];
}

Line::Point Pixel::getLeftDown() const {
    return borderPoints[3];
}

Pixel::Pixel(long double x, long double y) {
    center = std::make_pair(x, y);
    borderPoints[0] = std::make_pair(center.x - 0.5, center.y - 0.5);
    borderPoints[1] = std::make_pair(center.x + 0.5, center.y - 0.5);
    borderPoints[2] = std::make_pair(center.x + 0.5, center.y + 0.5);
    borderPoints[3] = std::make_pair(center.x - 0.5, center.y + 0.5);
    borders[0] = Line(getLeftUp(), getRightUp());
    borders[1] = Line(getRightUp(), getRightDown());
    borders[2] = Line(getRightDown(), getLeftDown());
    borders[3] = Line(getLeftDown(), getLeftUp());
}

const Line &Pixel::getUpLine() const {
    return borders[0];
}

const Line &Pixel::getDownLine() const {
    return borders[2];
}

const Line &Pixel::getLeftLine() const {
    return borders[3];
}

const Line &Pixel::getRightLine() const {
    return borders[1];
}

const Line::Point *Pixel::getBorderPoints() const {
    return borderPoints;
}

const Line *Pixel::getBorders() const {
    return borders;
}

bool Pixel::isPointInside(Line::Point point) const {
    return dEqual(getUpLine().distanceToPoint(point) + getDownLine().distanceToPoint(point), 1)
           && dEqual(getLeftLine().distanceToPoint(point) + getRightLine().distanceToPoint(point), 1);
}
