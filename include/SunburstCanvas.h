#pragma once

#include <GL/glut.h>
#include <functional>
#include <vector>

#include "Canvas.h"
#include "EntityData.h"
#include "Controller.h"
#include "Colormap.h"
#define PI 3.14159265359

extern Controller controller;
using namespace std;

class SunburstCanvas : public Canvas
{
public:
	SunburstCanvas(Point tl, Point br, EntityData *et);
	void drawCanvas(unsigned Rt, double animationStep);
	void getEntitiesByPosition(int *drag, unsigned click, bool ctrlDown);

private:
	void drawSlice(BaseEntity* b, unsigned Rt, double currentTheta);

	EntityData *entityData;
	double innerRadius;
	double unitWidth;
};
