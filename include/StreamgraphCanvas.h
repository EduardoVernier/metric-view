#pragma once

#include <GL/glut.h>
#include "Canvas.h"
#include "Entity.h"
#include "EntityData.h"
#include "Controller.h"
#include "Colormap.h"
#include "Color.h"

extern Controller controller;

using namespace std;

class StreamgraphCanvas : public Canvas
{
public:
	StreamgraphCanvas(Point tl, Point br, EntityData* et);
	void drawCanvas(unsigned Rt, double animationStep);
	void getEntitiesOnStreamgraph(int *drag, unsigned click, bool ctrlDown);

private:
	EntityData *entityData;
	int hoveredIndex = -1;
};
