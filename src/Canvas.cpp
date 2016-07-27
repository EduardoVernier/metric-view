#include "../include/Canvas.h"

void Canvas::setSize(Point tl, Point br)
{
	top_left = tl;
	bottom_right = br;
	if (firstMeasurement) // Solution for canvas scaling - let the scaling matrix do the work
	{
		xOff = tl.x;
		yOff = tl.y;
		firstMeasurement = 0;
	}
	currentWidth  = br.x - tl.x;
	currentHeight = br.y - tl.y;
}
