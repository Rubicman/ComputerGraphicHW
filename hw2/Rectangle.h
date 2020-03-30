#ifndef HW2_RECTANGLE_H
#define HW2_RECTANGLE_H


#include "Line.h"
#include "Pixel.h"

class Rectangle {
private:
    Line::Point points[4];
    Line sides[4];
    long double weight, height, thickness;
public:
    Rectangle(long double x1, long double y1, long double x2, long double y2, long double thickness);

    bool isPointInside(Line::Point point) const;

    long double intersectionArea(Pixel pixel) const;
};

long double getArea(std::vector<Line::Point> const &pointsList);

#endif //HW2_RECTANGLE_H
