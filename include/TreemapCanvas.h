#ifndef TREEMAPCANVAS_H
#define TREEMAPCANVAS_H

#include <vector>
#include <iostream>

#include "Canvas.h"
#include "EntityTree.h"
#include "Entity.h"
#include "Package.h"

using namespace std;

class TreemapCanvas : public Canvas
{
public:
  TreemapCanvas (Point tl, Point br, EntityTree et);
  void drawCanvas();

private:
  void drawEntity(BaseEntity* e);

  EntityTree entityTree;
};


#endif
