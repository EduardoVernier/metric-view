#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include "BaseEntity.h"

using namespace std;

class Container
{
public:
	Container(double _width, double _height, double _xOffset, double _yOffset);
	Container cutArea (double area);
	void saveCoordinates (vector<BaseEntity*> *row, double scoreSum); // Save coordinates INTO OBJECTS

	double width, height, xOffset, yOffset, shortestEdge;

};

#endif
