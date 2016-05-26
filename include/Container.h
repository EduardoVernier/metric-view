#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include "BaseEntity.h"

using namespace std; 

class Container
{
public:
	Container(int _width, int _height, int _xOffset, int _yOffset);
	Container cutArea (float area);
	void saveCoordinates (vector<BaseEntity*> *row, float scoreSum);


	int width, height, xOffset, yOffset, shortestEdge;

};

#endif