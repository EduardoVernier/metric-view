#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>

using namespace std; 

class Container
{
public:
	Container(int _width, int _height, int _xOffset, int _yOffset);

	int width, height, xOffset, yOffset, shortestEdge;

};

#endif