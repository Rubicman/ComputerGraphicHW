#ifndef HW2_LINE_H
#define HW2_LINE_H

#include <utility>

class Line {
public:
    typedef std::pair<double, double> Point;

    const Point &getVector() const;

    const Point &getPoint() const;

    Line();

    Line(Point point1, Point point2);

    friend Point intersectionPoint(Line line1, Line line2);

    friend bool isParallel(Line const &line1, Line const &line2);

    friend bool operator==(Line const &line1, Line const &line2);

    double distanceToPoint(Point point0) const;

    bool isPointOn(Point point0) const;

private:
    Point vector;
    Point point;
};

double distanceBetweenPoints(Line::Point point1, Line::Point point2);

bool dEqual(double a, double b);

int sign(double a);

#endif //HW2_LINE_H
