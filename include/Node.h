#pragma once

#include "Point.h"
#include "Vector2D.h"

class Node {
public:
    Point position = Point {0,0};
    Vector2D velocity = Vector2D{0,0};
    Vector2D netForce = Vector2D{0,0};
    bool movable;

    Node(bool movable) : movable(movable) {};
};