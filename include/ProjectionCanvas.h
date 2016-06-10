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


using namespace std;

class ProjectionCanvas : public Canvas
{
public:
  ProjectionCanvas(Point tl, Point br, EntityTree *et);
  void drawCanvas(unsigned Rt);

private:
  void drawFilledCircle(double x, double y, float radius, Color c);
  void normalizeProjection();

  EntityTree *entityTree;
};

#endif
