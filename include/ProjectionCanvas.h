#ifndef PROJECTIONCANVAS_H
#define PROJECTIONCANVAS_H

#define PI 3.14159265359

#include <vector>
#include <iostream>
#include <cmath>

#include "Canvas.h"
#include "Entity.h"
#include "EntityTree.h"
#include "Colormap.h"

extern int colormapIndex;
extern int deltaPie;
using namespace std;

class ProjectionCanvas : public Canvas
{
public:
	ProjectionCanvas(Point tl, Point br, EntityTree *et);
	void getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click, unsigned ctrlDown);
	void drawCanvas(unsigned Rt, double animationStep);

private:
	void drawEntity(double x, double y, float radius, float delta, Color c, int action);
	void normalizeProjection();
	void drawSolidEntity(double x, double y, float radius, Color c, int action);
	void drawPieEntity(double x, double y, float radius, float delta, Color c, int action);
	Point getPoint(Entity *e, unsigned Rt, double animationStep);

	EntityTree *entityTree;
};

#endif
