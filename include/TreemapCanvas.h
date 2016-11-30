#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Canvas.h"
#include "EntityData.h"
#include "Colormap.h"
#include "Controller.h"

extern Controller controller;
using namespace std;

class TreemapCanvas : public Canvas
{
public:
	TreemapCanvas (Point tl, Point br, EntityData *ed);
	void getEntitiesByPositionOnTreemap(int *drag, unsigned click, bool ctrlDown);
	void drawCanvas(unsigned Rt, double animationStep);

private:
	void drawHovered(BaseEntity *e);
	void drawSelected(Entity *e);
	void drawEntity(Entity *e, unsigned Rt, double animationStep);
	void drawHalo(const double *coords, double animationStep);
	void computeRectangleSize(double *retCoords, Entity *e, unsigned Rt, double animationStep);
	void drawPackageFirstLayer(BaseEntity *e);
	void drawPackageSecondLayer(BaseEntity *e);
	void labelCells();

	EntityData *entityData;
	double treemapXOff, treemapYOff;

	void drawSimilar();

	void linkEntities(Entity *a, Entity *b);

    void linkEntities(double dist, Entity *a, Entity *b);
};
