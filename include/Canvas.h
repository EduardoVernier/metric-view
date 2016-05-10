#ifndef CANVAS_H
#define CANVAS_H

#include "Point.h"
#include <GL/glut.h>
#include <GL/glui.h>

class Canvas
{
private:
    Point top_left, bottom_right;
public:
    Canvas(Point tl, Point br);
    void setSize(Point tl, Point br);
};
#endif
