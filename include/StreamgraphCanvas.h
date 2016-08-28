#pragma once

#include <GL/glut.h>
#include "Canvas.h"
#include "Entity.h"
#include "EntityTree.h"
#include "Controller.h"

extern Controller controller;

using namespace std;

class StreamgraphCanvas : public Canvas
{
public:
	StreamgraphCanvas(Point tl, Point br, EntityTree* et);
	void drawCanvas(unsigned Rt, double animationStep);
	void getEntitiesOnStreamgraph(int *drag, unsigned click, bool ctrlDown);

private:
	EntityTree *entityTree;
	int hoveredIndex = -1;
};
