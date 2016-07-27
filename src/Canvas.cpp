#include "../include/Canvas.h"

void Canvas::setSize(Point tl, Point br)
{
	top_left = tl;
	bottom_right = br;
	xOff = tl.x;
	yOff = tl.y;
	currentWidth  = br.x - tl.x;
	currentHeight = br.y - tl.y;
}
