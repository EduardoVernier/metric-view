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

	for (vector<Entity*>::iterator b = entityTree->selected.begin(); b != entityTree->selected.end(); ++b)
	{
		if((*b)->isPackage() == 0 && (*b)->getName() != "")
		{
			double x = ((Entity*)(*b))->normalizedProjectionPoints[Rt].x;
			double y = ((Entity*)(*b))->normalizedProjectionPoints[Rt].y;
			float value = ((Entity*)(*b))->data[Rt][rMetric];
			float radius = ((value) - rMin) / (rMax - rMin);
			drawFilledCircle(x, y, radius, colorSelection, 1);
		}
	}

	if (entityTree->hovered)
	{
		double x = entityTree->hovered->normalizedProjectionPoints[Rt].x;
		double y = entityTree->hovered->normalizedProjectionPoints[Rt].y;
		float value = entityTree->hovered->data[Rt][rMetric];
		float radius = ((value) - rMin) / (rMax - rMin);
		drawFilledCircle(x, y, radius, colorHover, 1);
	}

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

			drawFilledCircle(x, y, radius, c, 0);
		}
	}
}

// Draw circles
void ProjectionCanvas::drawFilledCircle(double x, double y, float radius, Color c, int action)
{
	x+=10; y+=10; // Compensate for border
	int triangleAmount = 100; //# of triangles used to draw circle
	GLfloat twicePi = 2.0f * PI;
	radius = radius*20 + 3 + action*4; // Scaling
	if (c == colorHover) radius += 2;

	glColor3f(c.R,c.G,c.B);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(x, y); // center of circle
	for(int i = 0; i <= triangleAmount;i++)
	{
		glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
							 y + (radius * sin(i * twicePi / triangleAmount)));
	}
	glEnd();


	if (action) // Hover or selection
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0,0,0);
		glVertex2d(x, y); // center of circle
		for(int i = 0; i <= triangleAmount;i++)
		{
			glVertex2f(x + ((radius-2) * cos(i * twicePi / triangleAmount)),
								 y + ((radius-2) * sin(i * twicePi / triangleAmount)));
		}
		glEnd();
	}
}
