#ifndef HW2_RECTANGLE_H
#define HW2_RECTANGLE_H


#include "Line.h"
#include "Pixel.h"

class Rectangle {
private:
    Line::Point points[4];
    Line sides[4];
    double weight, height, thickness;
public:
    Rectangle(double x1, double y1, double x2, double y2, double thickness);

    bool isPointInside(Line::Point point) const;

    double intersectionArea(Pixel pixel) const;
};

double getArea(std::vector<Line::Point> const &pointsList);

#endif //HW2_RECTANGLE_H
