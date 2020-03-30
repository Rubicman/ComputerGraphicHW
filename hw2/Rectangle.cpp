#include <algorithm>
#include <vector>
#include "Rectangle.h"

#define x first
#define y second

Rectangle::Rectangle(long double x1, long double y1, long double x2, long double y2, long double thickness) : thickness(
        thickness) {
    if (x1 == x2 && y1 == y2) {
        return;
    }
    Line line = Line(std::make_pair(x1, y1), std::make_pair(x2, y2));
    Line::Point vector = line.getVector();
    long double borderDistance = std::min(0.5l, thickness / 2);

    points[1] = std::make_pair(x1 - vector.x * borderDistance, y1 - vector.y * borderDistance);
    points[3] = std::make_pair(x2 + vector.x * borderDistance, y2 + vector.y * borderDistance);

    points[0] = std::make_pair(points[1].x - vector.y * thickness / 2, points[1].y + vector.x * thickness / 2);
    points[1] = std::make_pair(points[1].x + vector.y * thickness / 2, points[1].y - vector.x * thickness / 2);
    points[2] = std::make_pair(points[3].x + vector.y * thickness / 2, points[3].y - vector.x * thickness / 2);
    points[3] = std::make_pair(points[3].x - vector.y * thickness / 2, points[3].y + vector.x * thickness / 2);

    sides[0] = Line(points[0], points[1]);
    sides[1] = Line(points[1], points[2]);
    sides[2] = Line(points[2], points[3]);
    sides[3] = Line(points[3], points[0]);

    weight = distanceBetweenPoints(points[0], points[1]);
    height = distanceBetweenPoints(points[0], points[3]);
}

bool Rectangle::isPointInside(Line::Point point) const {
    return dEqual(sides[0].distanceToPoint(point) + sides[2].distanceToPoint(point), height) &&
           dEqual(sides[1].distanceToPoint(point) + sides[3].distanceToPoint(point), weight);
}

long double Rectangle::intersectionArea(Pixel pixel) const {
    if (thickness >= 1
        && !isPointInside(pixel.getLeftUp()) && !isPointInside(pixel.getRightUp())
        && !isPointInside(pixel.getLeftDown()) && !isPointInside(pixel.getRightDown())) {
        return 0;
    }
    if (isPointInside(pixel.getLeftUp()) && isPointInside(pixel.getRightUp())
        && isPointInside(pixel.getLeftDown()) && isPointInside(pixel.getRightDown())) {
        return 1;
    }
    for (Line::Point point : points) {
        if (pixel.isPointInside(point)) {
            return 0;
        }
    }
    long double s = 0;
    bool used[] = {false, false, false, false};
    const Line::Point *borderPoints = pixel.getBorderPoints();
    const Line *borders = pixel.getBorders();
    for (int i = 0; i < 4; i++) {
        if (used[i] || isPointInside(borderPoints[i])) {
            continue;
        }
        std::vector<Line::Point> pointList;
        int j = i;
        do {
            used[j] = true;
            pointList.push_back(borderPoints[j]);
            Line::Point nextPoint = borderPoints[(j + 1) % 4];
            Line nextLine = borders[j];
            for (Line side : sides) {
                Line::Point interPoint = intersectionPoint(side, borders[j]);
                if (borderPoints[j] != interPoint
                    && pixel.isPointInside(interPoint)
                    && distanceBetweenPoints(borderPoints[j], nextPoint) >
                       distanceBetweenPoints(borderPoints[j], interPoint)) {
                    nextPoint = interPoint;
                    nextLine = side;
                }
            }
            if (nextPoint == borderPoints[(j + 1) % 4]) {
                j = (j + 1) % 4;
            } else {
                pointList.push_back(nextPoint);
                for (int t = 0; t < 4; t++) {
                    if (t == j) {
                        continue;
                    }
                    Line::Point interPoint = intersectionPoint(nextLine, borders[t]);
                    if (pixel.isPointInside(interPoint) && interPoint != borderPoints[(t + 1) % 4]) {
                        pointList.push_back(interPoint);
                        j = (t + 1) % 4;
                        break;
                    }
                }
            }
        } while (!used[j]);
        s += getArea(pointList);
    }
    return 1 - std::abs(s);
}

long double getArea(const std::vector<Line::Point> &pointList) {
    if (pointList.empty()) {
        return 0;
    }
    long double s = 0;
    for (size_t i = 0; i < pointList.size() - 1; i++) {
        s += (pointList[i + 1].x - pointList[i].x) * (pointList[i].y + pointList[i + 1].y) / 2;
    }
    s += (pointList[0].x - pointList.back().x) * (pointList[0].y + pointList.back().y) / 2;
    return std::abs(s);
}

