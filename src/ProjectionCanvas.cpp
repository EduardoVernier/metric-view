#include "../include/ProjectionCanvas.h"

ProjectionCanvas::ProjectionCanvas(Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
	int shortEdge = ((br.x-tl.x) < (br.y-tl.y))? (br.x-tl.x) : (br.y-tl.y);
	entityTree->normalizeProjection(shortEdge);
}

//
void ProjectionCanvas::drawCanvas(unsigned Rt)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	int cMetric = entityTree->getColorMetric();
	float cMin = entityTree->getCMMin();
	float cMax = entityTree->getCMMax();

	int rMetric = entityTree->getRadiusMetric();
	float rMin = entityTree->getRMMin();
	float rMax = entityTree->getRMMax();

	for (vector<BaseEntity*>::iterator b = entityTree->sortedEntities.begin(); b != entityTree->sortedEntities.end(); ++b)
	{
		if((*b)->isPackage() == 0 && (*b)->getName() != "")
		{
			double x = ((Entity*)(*b))->normalizedProjectionPoints[Rt].x;
			double y = ((Entity*)(*b))->normalizedProjectionPoints[Rt].y;

			float value = ((Entity*)(*b))->data[Rt][cMetric];
			float normCValue = (value - cMin) / (cMax - cMin);
			Color c = sequentialColormap(normCValue);

			value = ((Entity*)(*b))->data[Rt][rMetric];
			float radius = (value - rMin) / (rMax - rMin);

			drawFilledCircle(x, y, radius, c);
		}
	}
	for (vector<Entity*>::iterator b = entityTree->selected.begin(); b != entityTree->selected.end(); ++b)
	{
		if((*b)->isPackage() == 0)
		{
			double x = ((Entity*)(*b))->normalizedProjectionPoints[Rt].x;
			double y = ((Entity*)(*b))->normalizedProjectionPoints[Rt].y;
			Color c = {0.0f,1.0f,0.0f};
			drawFilledCircle(x, y, 8.0, c);
		}
	}

	if (entityTree->hovered)
	{
		double x = entityTree->hovered->normalizedProjectionPoints[Rt].x;
		double y = entityTree->hovered->normalizedProjectionPoints[Rt].y;
		Color c = {0.0f,0.7f,0.0f};
		drawFilledCircle(x, y, 8.0, c);
	}
}

//
void ProjectionCanvas::drawFilledCircle(double x, double y, float radius, Color c)
{
	x+=10; y+=10; // Compensate for border
	int i;
	int triangleAmount = 100; //# of triangles used to draw circle
	GLfloat twicePi = 2.0f * PI;

	radius = radius*20 + 3; // Scaling

	glColor3f(c.R,c.G,c.B);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(x, y); // center of circle
	for(i = 0; i <= triangleAmount;i++)
	{
		glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
							 y + (radius * sin(i * twicePi / triangleAmount)));
	}
	glEnd();
}
