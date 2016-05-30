#ifndef TREEMAPCANVAS_H
#define TREEMAPCANVAS_H

#include <vector>
#include <iostream>
#include "math.h"

#include "Canvas.h"
#include "EntityTree.h"
#include "Entity.h"
#include "Package.h"

using namespace std;

struct Color
{
	float R, G, B;
};

class TreemapCanvas : public Canvas
{
public:
  TreemapCanvas (Point tl, Point br, EntityTree *et);
  void drawCanvas();

private:
  void drawEntity(BaseEntity *e);
  void drawPackage(BaseEntity *e);
  Color rainbow(double value);

  EntityTree *entityTree;
};


#endif
