#ifndef PROJECTIONCANVAS_H
#define PROJECTIONCANVAS_H

#include <vector>
#include <iostream>

#include "Canvas.h"
#include "Entity.h"
#include "EntityTree.h"
#include "Color.h"

using namespace std;

class ProjectionCanvas : public Canvas
{
public:
  ProjectionCanvas(Point tl, Point br, EntityTree *et);
  void drawCanvas();

private:
  EntityTree *entityTree;
};

#endif
