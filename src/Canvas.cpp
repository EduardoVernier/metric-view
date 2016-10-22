#include "../include/Canvas.h"

void Canvas::setSize(Point tl, Point br) {

    top_left = tl;
    bottom_right = br;
    yOff = tl.y;
    xOff = tl.x;
    currentWidth = br.x - tl.x;
    currentHeight = br.y - tl.y;

    if (initialWidth == -1 && initialHeight == -1) {
        initialWidth = currentWidth;
        initialHeight = currentHeight;
    }

}
