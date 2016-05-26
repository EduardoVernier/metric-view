#include "../include/Container.h"


Container::Container(int _width, int _height, int _xOffset, int _yOffset)
{
	width = _width;
	height = _height;
	xOffset = _xOffset;
	yOffset = _yOffset;

	shortestEdge = (width < height)? width : height;
}



Container Container::cutArea(float area)
{
	Container *newContainer;

	if (width >= height) 
	{
		float areaWidth = area / height;
		float newWidth = width - areaWidth;
		newContainer = new Container(newWidth, height, xOffset + areaWidth, yOffset);
	} 
	else 
	{
		float areaHeight = area / width;
		float newHeight = height - areaHeight;
		newContainer = new Container(width, newHeight, xOffset, yOffset + areaHeight);
	}
	return *newContainer;	
}
