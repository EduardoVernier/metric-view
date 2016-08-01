#ifndef STREAMGRAPHCANVAS_H
#define STREAMGRAPHCANVAS_H

#include <GL/glut.h>
#include "Canvas.h"
#include "Entity.h"
#include "EntityTree.h"

extern int streamgraphHeight;

using namespace std;

class StreamgraphCanvas : public Canvas
{
public:
	StreamgraphCanvas(Point tl, Point br, EntityTree* et);
	void drawCanvas(unsigned Rt);

private:
	EntityTree *entityTree;
};
#endif
