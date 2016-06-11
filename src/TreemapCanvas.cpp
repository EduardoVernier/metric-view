#include "../include/TreemapCanvas.h"

TreemapCanvas::TreemapCanvas (Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
}

// First draw elements and then package borders
void TreemapCanvas::drawCanvas(unsigned Rt)
{
	vector<BaseEntity*> items = entityTree->sortedEntities;

	for (vector<BaseEntity*>::iterator it = entityTree->sortedEntities.begin(); it != entityTree->sortedEntities.end(); ++it)
	{
		if ((*it)->isPackage() == 0)
		{
			drawEntity(*it);
		}
	}

	for (vector<BaseEntity*>::iterator it = entityTree->sortedEntities.begin(); it != entityTree->sortedEntities.end(); ++it)
	{
		if ((*it)->isPackage())
		{
			drawPackage(*it);
		}
	}

	for (vector<Entity*>::iterator it = entityTree->selected.begin(); it != entityTree->selected.end(); ++it)
	{
			drawSelected(*it);
	}

	drawHovered(entityTree->hovered);
}

void TreemapCanvas::drawEntity(BaseEntity *e)
{
	double padding = 0.5;
	double x0 = e->getCoord(0)+ padding + xOff;
	double y0 = e->getCoord(1)+ padding + yOff;
	double x1 = e->getCoord(2)- padding + xOff;
	double y1 = e->getCoord(3)- padding + yOff;

	double minT = entityTree->getMin(), maxT = entityTree->getMax();
	double value = e->getScore() * (1.0/(maxT-minT)) - minT/(maxT-minT);
	Color c = sequentialColormap(value);
	glColor3f(c.R, c.G, c.B);
	glRectd(x0,y0,x1,y1);
}

void TreemapCanvas::drawPackage(BaseEntity *e)
{
	double x0 = e->getCoord(0) + xOff;
	double y0 = e->getCoord(1) + yOff;
	double x1 = e->getCoord(2) + xOff;
	double y1 = e->getCoord(3) + yOff;

	glLineWidth(2.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y0);
	glVertex2d(x0,y0);
	glEnd();
}

void TreemapCanvas::drawHovered(Entity *e)
{
	if (e == NULL) return;

	double padding = 0.0;
	double x0 = e->getCoord(0)+ padding + xOff;
	double y0 = e->getCoord(1)+ padding + yOff;
	double x1 = e->getCoord(2)- padding + xOff;
	double y1 = e->getCoord(3)- padding + yOff;

	glEnable(GL_BLEND);
	glColor4f(0.0, 0.0, 0.0, 0.5);
	glRectd(x0,y0,x1,y1);
	glDisable(GL_BLEND);
}

void TreemapCanvas::drawSelected(Entity *e)
{
	double padding = 0.0;
	double x0 = e->getCoord(0)+ padding + xOff;
	double y0 = e->getCoord(1)+ padding + yOff;
	double x1 = e->getCoord(2)- padding + xOff;
	double y1 = e->getCoord(3)- padding + yOff;

	Color c = rainbow(e->getScore());
	glColor3f(c.R*0.7, c.G*0.7, c.B*0.7);
	glBegin(GL_TRIANGLES);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1);
	glVertex2d(x1,y1);
	glEnd();
}

// Map normalized (0:1) scalar to a color on the rainbow colormap
Color TreemapCanvas::rainbow(double value)
{
	double minT = entityTree->getMin(), maxT = entityTree->getMax();
	value = value * (1.0/(maxT-minT)) - minT/(maxT-minT);

	const float dx=0.8;
	if (value<0)
			value = 0;
	if (value>1)
			value = 1;

	value = (6-2*dx)*value+dx;
	Color c (max(0.0,(3-fabs(value-4)-fabs(value-5))/2),
	c.G = max(0.0,(4-fabs(value-2)-fabs(value-4))/2),
	c.B = max(0.0,(3-fabs(value-1)-fabs(value-2))/2));

	return c;
}


void TreemapCanvas::renderString(int x, int y, string str, Color c)
{
	glColor3f(c.R, c.G, c.B);
	glRasterPos2i(x, y);
	const unsigned char* s = reinterpret_cast<const unsigned char *>(str.c_str());
	glutBitmapString(GLUT_BITMAP_9_BY_15, s);
}
