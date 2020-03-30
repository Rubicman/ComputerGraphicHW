#ifndef HW2_LINE_H
#define HW2_LINE_H

#include <utility>

class Line {
public:
    typedef std::pair<long double, long double> Point;

    const Point &getVector() const;

    const Point &getPoint() const;

    Line();

    Line(Point point1, Point point2);

    friend Point intersectionPoint(Line line1, Line line2);

    friend bool isParallel(Line const &line1, Line const &line2);

    friend bool operator==(Line const &line1, Line const &line2);

    long double distanceToPoint(Point point0) const;

    bool isPointOn(Point point0) const;

private:
    Point vector;
    Point point;
};

long double distanceBetweenPoints(Line::Point point1, Line::Point point2);

bool dEqual(long double a, long double b);

int sign(long double a);

#endif //HW2_LINE_H
