#pragma once

#define PI 3.14159265359
#define MIN_RADIUS 3
#define MAX_RADIUS 23

#include <vector>
#include <iostream>
#include <cmath>
#include <functional>
#include <iomanip>

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
	static ProjectionCanvas& getInstance();
	void init(Point tl, Point br, EntityData *ed);
	void getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click, bool ctrlDown);
	void drawCanvas(unsigned Rt, double animationStep);

private:
	ProjectionCanvas();
	ProjectionCanvas(ProjectionCanvas const&);
	void operator=(ProjectionCanvas const&);
	void drawEntity(double x, double y, double radius, double delta, Color color, int action);
	void drawHalo(double x, double y, double radius, double animationStep);
	void drawSolidEntity(double x, double y, double radius, Color c, int action);
	void drawPieEntity(double x, double y, double radius, double delta, Color c, int action);
	void drawCircle(double x, double y, double z, double radius, Color color);
	Point getPoint(Entity *e, unsigned Rt, double animationStep);
	void displayRadiusLegend();
	void displayColorbar();

	EntityData *entityData;
	double minRatio;

};
