#include "../include/ProjectionCanvas.h"

ProjectionCanvas::ProjectionCanvas(Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
	int shortEdge = ((br.x-tl.x) < (br.y-tl.y))? (br.x-tl.x) : (br.y-tl.y);
	entityTree->normalizeProjection(shortEdge-50);
	initialWidth  = br.x - tl.x;
	initialHeight = br.y - tl.y;
}

//
void ProjectionCanvas::drawCanvas(unsigned Rt)
{
	// Scale initial aspect ratio by new
	glPushMatrix();
	double xRatio = double(currentWidth)/double(initialWidth);
	double yRatio = double(currentHeight)/double(initialHeight);
	glScaled(xRatio, yRatio, 1);

	glEnable(GL_LINE_SMOOTH);
	glColor3f(1.0f, 1.0f, 1.0f);
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
			float delta = (Rt > 1) ? (value - ((Entity*)(*b))->data[Rt-1][rMetric])/value: 0;
			drawEntity(x, y, radius, delta, colorSelection, 1);
		}
	}

	if (entityTree->hovered)
	{
		double x = entityTree->hovered->normalizedProjectionPoints[Rt].x;
		double y = entityTree->hovered->normalizedProjectionPoints[Rt].y;
		float value = entityTree->hovered->data[Rt][rMetric];
		float radius = ((value) - rMin) / (rMax - rMin);
		float delta = (Rt > 1) ? (value - ((Entity*)(entityTree->hovered))->data[Rt-1][rMetric])/value: 0;
		//cout << delta << endl;
		drawEntity(x, y, radius, delta, colorHover, 1);
	}

	for (vector<BaseEntity*>::iterator b = entityTree->sortedEntities.begin(); b != entityTree->sortedEntities.end(); ++b)
	{
		if((*b)->isPackage() == 0 && (*b)->getName() != "")
		{
			double x = ((Entity*)(*b))->normalizedProjectionPoints[Rt].x;
			double y = ((Entity*)(*b))->normalizedProjectionPoints[Rt].y;

			float value = ((Entity*)(*b))->data[Rt][cMetric];
			float normCValue = (value - cMin) / (cMax - cMin);

			// Generate color based on colormap index value
			Color c (1,1,1);
			switch (colormapIndex)
			{
				case 0:
					c = sequentialColormap(normCValue);
					break;
				case 1:
					c = qualitativeColormap((*b)->firstLevelId);
					break;
				case 2:
					if (Rt > 0)
					{
						float pValue = ((Entity*)(*b))->data[Rt-1][cMetric];
						float pNormCValue = ((pValue - cMin) / (cMax - cMin));
						c = divergentColormap(normCValue-pNormCValue);
					}
					break;
			}

			float radius = (value - rMin) / (rMax - rMin);
			float delta = (Rt > 1) ? (value - ((Entity*)(*b))->data[Rt-1][rMetric])/value: 0;
			drawEntity(x, y, radius, delta, c, 0);
		}
	}
	glDisable(GL_LINE_SMOOTH);
	glPopMatrix();
}

// Draw circles
void ProjectionCanvas::drawEntity(double x, double y, float radius, float delta, Color c, int action)
{
	if (deltaPie == 0 || fabs(delta) < 0.01) // 1% tolerance
		drawSolidEntity(x+10, y+10, radius, c, action);
	else
		drawPieEntity(x+10, y+10, radius, delta, c, action);
}

void ProjectionCanvas::drawSolidEntity(double x, double y, float radius, Color c, int action)
{
	int triangleAmount = 360; //# of triangles/degrees used to draw circle

	radius = radius*20 + 3 + action*4; // Scaling
	if (c == colorHover)
		radius += 2;

	GLfloat radians = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(c.R,c.G,c.B);
	glVertex3f(x, y, 0);
	for(int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0,0,0);
	for(int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();
}


void ProjectionCanvas::drawPieEntity(double x, double y, float radius, float delta, Color c, int action)
{
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

	// Draw stroke
	glBegin(GL_LINE_STRIP);
	glColor3f(0,0,0);
	glVertex3f(x, y, 0);
	for(int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glVertex3f(x, y, 0);
	glEnd();

	glPopMatrix();

	// Draw missing slice
	if (delta > 0)
		radius *= 1.2;
	else
		radius = 0;

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

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(x, y ,0); // Translate vector to the object's position

	// Define removed slice accordingly to delta value
	if (delta > 0)
		rotation = -90 - delta*180;
	else
		rotation = 90 + delta*180;

	glRotatef(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

	glTranslatef(-x, -y ,0); // Translate to normal origin

	// Draw missing slice
	glBegin(GL_LINE_STRIP);
	glLineWidth(2.0);
	glVertex3f(x, y, 0);
	for(int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glVertex3f(x, y, 0);
	glEnd();
	glPopMatrix();
}
