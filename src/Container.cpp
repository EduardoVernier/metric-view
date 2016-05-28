#include "../include/Container.h"


Container::Container(double _width, double _height, double _xOffset, double _yOffset)
{
	width = _width;
	height = _height;
	xOffset = _xOffset;
	yOffset = _yOffset;

	shortestEdge = (width < height)? width : height;
}



Container Container::cutArea(double area)
{
	Container *newContainer;

	if (width >= height)
	{
		double areaWidth = area / height;
		double newWidth = width - areaWidth;
		newContainer = new Container(newWidth, height, xOffset + areaWidth, yOffset);
	}
	else
	{
		double areaHeight = area / width;
		double newHeight = height - areaHeight;
		newContainer = new Container(width, newHeight, xOffset, yOffset + areaHeight);
	}
	return *newContainer;
}

void Container::saveCoordinates (vector<BaseEntity*> *row, double scoreSum)
{
	double subxOffset = xOffset, subyOffset = yOffset; // Offset within the container
	double areaWidth = scoreSum / height;
	double areaHeight = scoreSum / width;
	double coordinates [4];

	if (width >= height)
	{
		for (unsigned i = 0; i < row->size(); i++)
		{
			coordinates[0] = subxOffset;
			coordinates[1] = subyOffset;
			coordinates[2] = subxOffset + areaWidth;
			coordinates[3] = subyOffset + (*row)[i]->getNormalizedScore() / areaWidth;
			(*row)[i]->setCoords(coordinates);
			subyOffset = subyOffset + (*row)[i]->getNormalizedScore() / areaWidth;
		}
	}
	else
	{
		for (unsigned i = 0; i < row->size(); i++)
		{
			coordinates[0] = subxOffset;
			coordinates[1] = subyOffset;
			coordinates[2] = subxOffset + (*row)[i]->getNormalizedScore() / areaHeight;
			coordinates[3] = subyOffset + areaHeight;
			(*row)[i]->setCoords(coordinates);
			subxOffset = subxOffset + (*row)[i]->getNormalizedScore() / areaHeight;
		}
	}
};
