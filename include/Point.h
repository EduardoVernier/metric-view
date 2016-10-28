#pragma once

#include <cmath>

class Point
{
public:

    double x, y;
    Point() : x(0), y(0) {};
    Point(double px, double py) : x(px), y(py) {};
    Point operator+(const Point v) { return Point(x + v.x, y + v.y); };
    Point& operator += (const Point& b)
    {
        this->x += b.x;
        this->y += b.y;
        return *this;
    }
    Point operator-(const Point v) { return Point(x - v.x, y - v.y); };
    Point operator*(const double &b) { return Point(x * b, y * b); };
    Point operator/(const double &b) { return Point(x / b, y / b); };
	double euclidianDistance(const Point b) { return sqrt(pow(x - b.x, 2) + pow(y - b.y, 2)); }
};
