#ifndef TREEMAPCANVAS_H
#define TREEMAPCANVAS_H

#include <vector>
#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Canvas.h"
#include "Entity.h"
#include "Package.h"
#include "EntityTree.h"
#include "Colormap.h"

extern int colormapIndex;
extern int dynamicTreemap;

using namespace std;

class TreemapCanvas : public Canvas
{
public:
	TreemapCanvas (Point tl, Point br, EntityTree *et);
	void getEntitiesByPositionOnTreemap(int *drag, unsigned click, unsigned ctrlDown);
	void drawCanvas(unsigned Rt, double animationStep);

private:
	void drawHovered(Entity *e);
	void drawSelected(Entity *e);
	void drawEntity(Entity *e, unsigned Rt, double animationStep);
	void computeRectangleSize(double *retCoords, Entity *e, unsigned Rt, double animationStep);
	void drawPackage(BaseEntity *e);
	void labelCells();

	EntityTree *entityTree;
};


#endif
