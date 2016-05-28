#include "../include/Canvas.h"

void Canvas::setSize(Point tl, Point br)
{
    top_left = tl;
    bottom_right = br;
}

void Canvas::drawCanvas()
{
    glRecti(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
}
