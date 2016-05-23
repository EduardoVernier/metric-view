#include "../include/Container.h"


Container::Container(int _width, int _height, int _xOffset, int _yOffset)
{
	width = _width;
	height = _height;
	xOffset = _xOffset;
	yOffset = _yOffset;

	shortestEdge = (width < height)? width : height;
}

