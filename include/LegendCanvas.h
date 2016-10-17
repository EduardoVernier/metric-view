#pragma once

#include <GL/glui.h>

#include "Canvas.h"
#include "EntityData.h"
#include "ProjectionCanvas.h"
#include "Controller.h"
extern Controller controller;

class LegendCanvas : public Canvas
{
public:
	static LegendCanvas& getInstance();
	void init(Point tl, Point br, EntityData *ed);
	void drawCanvas(unsigned Rt, double animationStep);

private:
	LegendCanvas();
	LegendCanvas(LegendCanvas const&);
	void operator=(LegendCanvas const&);

	EntityData *entityData;
};