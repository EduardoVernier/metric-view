#pragma once

#define PI 3.14159265359

#include <vector>
#include <iostream>
#include <cmath>
#include <functional>

#include "Canvas.h"
#include "Entity.h"
#include "EntityData.h"
#include "Colormap.h"
#include "Controller.h"

extern Controller controller;
using namespace std;

class ProjectionCanvas : public Canvas
{
public:
	ProjectionCanvas(Point tl, Point br, EntityData *ed);
	void getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click, bool ctrlDown);
	void drawCanvas(unsigned Rt, double animationStep);

private:
	void drawEntity(double x, double y, double radius, double delta, Color c, int action);
	void drawHalo(double x, double y, double radius, double animationStep);
	void drawSolidEntity(double x, double y, double radius, Color c, int action);
	void drawPieEntity(double x, double y, double radius, double delta, Color c, int action);
	Point getPoint(Entity *e, unsigned Rt, double animationStep);

	EntityData *entityData;
};
