#pragma once

#include <GL/glut.h>
#include "Canvas.h"
#include "Entity.h"
#include "EntityTree.h"
#include "Controller.h"
#include "Colormap.h"
#include "Color.h"

#define MAX_LINE_HEIGHT 10

extern Controller controller;

using namespace std;

class SpectrographCanvas : public Canvas
{
public:
	SpectrographCanvas(Point tl, Point br, EntityTree* et);
	void drawCanvas(unsigned Rt, double animationStep);
	int getHeight();
	void updateLocalSelectedGroup();

private:
	vector<pair<double,Entity*>> sortedSelectedEntities;
	EntityTree *entityTree;
};
