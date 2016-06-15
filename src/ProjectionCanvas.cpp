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
			drawEntity(x, y, radius, colorSelection, 1);
		}
	}

	if (entityTree->hovered)
	{
		double x = entityTree->hovered->normalizedProjectionPoints[Rt].x;
		double y = entityTree->hovered->normalizedProjectionPoints[Rt].y;
		float value = entityTree->hovered->data[Rt][rMetric];
		float radius = ((value) - rMin) / (rMax - rMin);
		drawEntity(x, y, radius, colorHover, 1);
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

			// Use qualitative colormap if ui checkbox is checked
			if (hierarchicalColoring)
				c = qualitativeColormap((*b)->firstLevelId);

			value = ((Entity*)(*b))->data[Rt][rMetric];
			float radius = (value - rMin) / (rMax - rMin);

			drawEntity(x, y, radius, c, 0);
		}
	}
}

// Draw circles
void ProjectionCanvas::drawEntity(double x, double y, float radius, Color c, int action)
{
	float delta = 0.2;
	int deltaPie = 1;

	x+=10; y+=10; // Compensate for border
	int triangleAmount = 360; //# of triangles/degrees used to draw circle

	radius = radius*20 + 3 + action*4; // Scaling
	if (c == colorHover)
		radius += 2;

	GLfloat radians = 2.0f * PI;
	if (deltaPie)
		radians *= (1 - fabs(delta)); // Take only a fraction of the 360 degrees


	// Old indian trick to rotate a circle around it's center
	glPushMatrix();
	glColor3f(c.R,c.G,c.B);
	float rotation = 0;
	glTranslatef(x, y ,0); // Translate vector to the object's position

	// Define removed slice accordingly to delta value
	if (delta > 0)
		rotation = -90 + delta*180;
	else
		rotation = 90 - delta*180;

	glRotatef(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

	glTranslatef(-x, -y ,0); // Translate to normal origin

	// Draw circle with missing slice
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, 0);
	for(int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();
	glPopMatrix();

	// Draw missing slice
	if (delta > 0)
		radius *= 1.2;
	else
		radius *= 0.9;

	radians = 2*PI - radians;

	glPushMatrix();
	glColor3f(c.R,c.G,c.B);
	glTranslatef(x, y ,0); // Translate vector to the object's position

	// Define removed slice accordingly to delta value
	if (delta > 0)
		rotation = -90 - delta*180;
	else
		rotation = 90 + delta*180;

	glRotatef(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

	glTranslatef(-x, -y ,0); // Translate to normal origin

	// Draw missing slice
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, 0);
	for(int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();
	glPopMatrix();


	if (action) // Hover or selection
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0,0,0);
		glVertex2d(x, y); // center of circle
		for(int i = 0; i <= triangleAmount;i++)
		{
			glVertex2f(x + ((radius-2) * cos(i * radians / triangleAmount)),
								 y + ((radius-2) * sin(i * radians / triangleAmount)));
		}
		glEnd();
	}
	glPopMatrix();
}
