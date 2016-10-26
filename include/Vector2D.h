#pragma once

#include "HierarchicalGraph.h"
#include "HierarchicalGraph.h"

class Vector2D {

public:
    double x;
    double y;

    Vector2D(double x, double y) : x(x), y(y) {}
    Vector2D operator+(const Vector2D v) { return Vector2D(x + v.x, y + v.y); }
    Vector2D operator*(const double &b) { return Vector2D(x * b, y * b); }
    Vector2D operator/(const double &b) { return Vector2D(x / b, y / b); }
};

