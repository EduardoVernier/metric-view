#ifndef TREEMAPCANVAS_H
#define TREEMAPCANVAS_H

#include <vector>
#include <iostream>
#include "math.h"

#include "Canvas.h"
#include "Entity.h"
#include "Package.h"
#include "EntityTree.h"
#include "Color.h"

using namespace std;

class TreemapCanvas : public Canvas
{
public:
  TreemapCanvas (Point tl, Point br, EntityTree *et);
	void renderString(int x, int y, string str, Color c);
	void drawCanvas(unsigned Rt);

private:
  void drawEntity(BaseEntity *e);
  void drawPackage(BaseEntity *e);
  Color rainbow(double value);

  EntityTree *entityTree;
};


#endif
