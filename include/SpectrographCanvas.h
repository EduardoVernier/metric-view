#pragma once

#include <GL/glut.h>
#include <iostream>

#include "Canvas.h"
#include "Entity.h"
#include "EntityData.h"
#include "Controller.h"
#include "Colormap.h"
#include "Color.h"
#define MAX_LINE_HEIGHT 15

extern Controller controller;

class SpectrographCanvas : public Canvas
{
public:
	static SpectrographCanvas& getInstance();
	void init(Point tl, Point br, EntityData *et);

	void drawCanvas(unsigned Rt, double animationStep);
	double getHeight();
	void updateLocalSelectedGroup();
	void getEntitiesOnSpectrograph(int *drag, unsigned click, bool ctrlDown);

private:
	SpectrographCanvas();
	SpectrographCanvas(SpectrographCanvas const&);
	void operator=(SpectrographCanvas const&);

	vector<std::pair<double,Entity*>> sortedSelectedEntities;
	EntityData *entityData;
};
