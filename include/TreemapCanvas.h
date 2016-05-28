#ifndef TREEMAPCANVAS_H
#define TREEMAPCANVAS_H

#include "Canvas.h"
#include "EntityTree.h"

using namespace std;

class TreemapCanvas : public Canvas
{
public:
  TreemapCanvas (Point tl, Point br, EntityTree *et);
};


#endif
