#ifndef HW2_PIXEL_H
#define HW2_PIXEL_H


#include <utility>
#include "Line.h"

class Pixel {
private:
    Line::Point center;
    Line::Point borderPoints[4];
    Line borders[4];
public:
    Line::Point getRightUp() const;

    Line::Point getRightDown() const;

    Line::Point getLeftUp() const;

    Line::Point getLeftDown() const;

    Pixel(long double x, long double y);

    const Line &getUpLine() const;

    const Line &getDownLine() const;

    const Line &getLeftLine() const;

    const Line &getRightLine() const;

    const Line::Point *getBorderPoints() const;

    const Line *getBorders() const;

    bool isPointInside(Line::Point point) const;
};


#endif //HW2_PIXEL_H
