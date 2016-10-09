#pragma once

#include <GL/glut.h>
#include <iostream>

#include "Canvas.h"
#include "Entity.h"
#include "EntityTree.h"
#include "Controller.h"
#include "Colormap.h"
#include "Color.h"
#define MAX_LINE_HEIGHT 15

extern Controller controller;

class SpectrographCanvas : public Canvas
{
public:
	SpectrographCanvas(Point tl, Point br, EntityTree* et);
	void drawCanvas(unsigned Rt, double animationStep);
	int getHeight();
	void updateLocalSelectedGroup();
	void getEntitiesOnSpectrograph(int *drag, unsigned click, bool ctrlDown);

private:
	vector<std::pair<double,Entity*>> sortedSelectedEntities;
	EntityTree *entityTree;
};
