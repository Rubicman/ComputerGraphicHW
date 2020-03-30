#include "Line.h"

#include <utility>
#include <cmath>

#define x first
#define y second

const Line::Point &Line::getVector() const {
    return vector;
}

const Line::Point &Line::getPoint() const {
    return point;
}

Line::Line(Line::Point point1, Line::Point point2)
        : point(std::move(point1)) {
    vector = std::make_pair(point2.x - point1.x, point2.y - point1.y);
    double length = sqrtl(this->vector.x * this->vector.x + this->vector.y * this->vector.y);
    this->vector.x /= length;
    this->vector.y /= length;
}

Line::Point intersectionPoint(Line line1, Line line2) {
    if (isParallel(line1, line2)) {
        return std::make_pair(nan("0"), nan("0"));
    }
    if (line1.vector.x == 0 || line2.vector.y == 0) {
        std::swap(line1, line2);
    }
    auto vector = line1.vector;
    double a = line2.point.x - line1.point.x;
    double b = line2.point.y - line1.point.y;

    line2.vector.x /= line1.vector.x;
    a /= line1.vector.x;
    line1.vector.x = 1;

    line2.vector.y -= line2.vector.x * line1.vector.y;
    b -= a * line1.vector.y;
    line1.vector.y = 0;

    b /= line2.vector.y;
    a -= line2.vector.x * b;
    return std::make_pair(
            line1.point.x + vector.x * a,
            line1.point.y + vector.y * a);
}

bool isParallel(Line const &line1, Line const &line2) {
    return (line1.vector.x == 0 && line2.vector.x == 0)
           || (line1.vector.y == 0 && line2.vector.y == 0)
           || (line1.vector.x / line2.vector.x == line1.vector.y / line2.vector.y);
}

Line::Line() {
    vector = std::make_pair(1.0, 0.0);
    point = std::make_pair(0.0, 0.0);
}

bool operator==(Line const &line1, Line const &line2) {
    if (line1.point == line2.point) {
        return isParallel(line1, line2);
    }
    Line line = Line(std::make_pair(line1.point.x - line2.point.x, line1.point.y - line2.point.y), line1.point);
    return isParallel(line1, line2) && isParallel(line1, line);
}

double Line::distanceToPoint(Line::Point point0) const {
    if (isPointOn(point0)) {
        return 0;
    }
    Line line;
    line.point = point0;
    line.vector.x = vector.y;
    line.vector.y = -vector.x;
    return distanceBetweenPoints(point0, intersectionPoint(*this, line));
}

bool Line::isPointOn(Line::Point point0) const {
    return (*this) == Line(point, point0);
}

double distanceBetweenPoints(Line::Point point1, Line::Point point2) {
    return powl((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y), 0.5);
}

bool dEqual(double a, double b) {
    const double eps = 1e-12;
    return (a > b - eps && a < b + eps);
}

int sign(double a) {
    return (0 < a) - (a < 0);
}
