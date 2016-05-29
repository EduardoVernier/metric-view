#ifndef CANVAS_H
#define CANVAS_H

#include <GL/glut.h>
#include <GL/glui.h>

#include "Point.h"

class Canvas
{
protected:
    Point top_left, bottom_right;
    double xOff, yOff;
public:
    void setSize(Point tl, Point br);
    virtual void drawCanvas() = 0;
};
#endif
