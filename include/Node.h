#pragma once

#include "Point.h"

class Node {
public:
    Point position = Point {0,0};
    Point velocity = Point {0,0};
    Point netForce = Point {0,0};
    Entity *entity = NULL;
    bool movable;

    Node(bool movable) : movable(movable) {};
};