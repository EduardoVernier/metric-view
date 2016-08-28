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

// Mark entities as selected from projection pane interaction
void ProjectionCanvas::getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click, bool ctrlDown)
{
	Entity *closest = NULL;
	entityTree->hovered = NULL;
	double smallerDist = FLT_MAX;

	if (click && !ctrlDown)
		entityTree->selected.clear();

	for (vector<BaseEntity*>::iterator b = entityTree->sortedEntities.begin(); b != entityTree->sortedEntities.end(); ++b)
	{
		if ((*b)->isPackage() == 0 && (*b)->getName() != "")
		{
			double bx = ((Entity*)(*b))->normalizedProjectionPoints[Rt].x * xRatio;
			double by = ((Entity*)(*b))->normalizedProjectionPoints[Rt].y * yRatio;
			if ((drag[0] == drag[2] && drag[1] == drag[3])) // Case point click (or hover of click = 0)
			{
				double distX = drag[0] - bx;
				double distY = drag[1] - by;
				double dist = sqrt(pow(distX,2) + pow(distY,2));
				if (dist < 10 && dist < smallerDist) // If click is close enough
				{
					if (click)
					{
						smallerDist = dist;
						closest = (Entity*)(*b);
					}
					else
					{
						smallerDist = dist;
						entityTree->hovered = (Entity*)(*b);
					}
				}
			}
			else if (bx > drag[0] && bx < drag[2] && by > drag[1] && by < drag[3]) // If inside selection box
			{
				if ((std::find(entityTree->selected.begin(), entityTree->selected.end(),(Entity*)*b))!= entityTree->selected.end())
					entityTree->selected.erase(std::find(entityTree->selected.begin(), entityTree->selected.end(),(Entity*)*b));
				else
					entityTree->selected.push_back((Entity*)*b);
			}
		}
	}
	if (click == 1 && closest != NULL)
	{
		if ((std::find(entityTree->selected.begin(), entityTree->selected.end(),closest))!=entityTree->selected.end())
			entityTree->selected.erase(std::find(entityTree->selected.begin(), entityTree->selected.end(),closest));
		else
			entityTree->selected.push_back(closest);
	}
}

// Draw scaled Canvas
void ProjectionCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	// Scale initial aspect ratio by new
	glPushMatrix();
	xRatio = double(currentWidth)/double(initialWidth);
	yRatio = double(currentHeight)/double(initialHeight);
	glScaled(xRatio, yRatio, 1);

	glEnable(GL_LINE_SMOOTH);

	int cMetric = entityTree->getColorMetric();
	float cMin = entityTree->getCMMin();
	float cMax = entityTree->getCMMax();

	int rMetric = entityTree->getRadiusMetric();
	float rMin = entityTree->getRMMin();
	float rMax = entityTree->getRMMax();

	for (vector<Entity*>::iterator b = entityTree->selected.begin(); b != entityTree->selected.end(); ++b)
	{
		if ((*b)->isPackage() == 0 && (*b)->getName() != "")
		{
			Point p = getPoint((Entity*)(*b), Rt, animationStep);
			float value = ((Entity*)(*b))->data[Rt][rMetric];
			float radius = ((value) - rMin) / (rMax - rMin);
			float delta = (Rt > 1) ? (value - ((Entity*)(*b))->data[Rt-1][rMetric])/value: 0;
			drawEntity(p.x, p.y, radius, delta, colorSelection, 1);
		}
	}

	if (entityTree->hovered)
	{
		Point p = getPoint(entityTree->hovered, Rt, animationStep);
		float value = entityTree->hovered->data[Rt][rMetric];
		float radius = ((value) - rMin) / (rMax - rMin);
		float delta = (Rt > 1) ? (value - ((Entity*)(entityTree->hovered))->data[Rt-1][rMetric])/value: 0;
		drawEntity(p.x, p.y, radius, delta, colorHover, 1);
	}

	for (vector<BaseEntity*>::iterator b = entityTree->sortedEntities.begin(); b != entityTree->sortedEntities.end(); ++b)
	{
		if ((*b)->isPackage() == 0 && (*b)->getName() != "")
		{
			Point p = getPoint((Entity*)(*b), Rt, animationStep);
			float value = ((Entity*)(*b))->data[Rt][cMetric];
			float normCValue = (value - cMin) / (cMax - cMin);

			// Generate color based on colormap index value
			Color c (1,1,1);
			switch (controller.colormapIndex)
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

			value = ((Entity*)(*b))->data[Rt][rMetric];
			float radius = (value - rMin) / (rMax - rMin);
			float delta = (Rt > 1) ? (value - ((Entity*)(*b))->data[Rt-1][rMetric])/value: 0;
			drawEntity(p.x, p.y, radius, delta, c, 0);
		}
	}
	glDisable(GL_LINE_SMOOTH);
	glPopMatrix();
}

// Draw circles
void ProjectionCanvas::drawEntity(double x, double y, float radius, float delta, Color c, int action)
{
	if (controller.deltaPie == 0 || fabs(delta) < 0.01) // 1% tolerance
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
	for (int i = 0; i <= triangleAmount;i++)
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0,0,0);
	for (int i = 0; i <= triangleAmount;i++)
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
	if (controller.deltaPie)
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
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();

	// Draw stroke
	glBegin(GL_LINE_STRIP);
	glColor3f(0,0,0);
	glVertex3f(x, y, 0);
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
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
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
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
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glVertex3f(x, y, 0);
	glEnd();
	glPopMatrix();
}

// Interpolates points according to animation step
Point ProjectionCanvas::getPoint(Entity *b, unsigned Rt, double animationStep)
{
	Point p;
	if (animationStep == 1 || animationStep == -1) // No movement
	{
		p.x = b->normalizedProjectionPoints[Rt].x;
		p.y = b->normalizedProjectionPoints[Rt].y;
	}
	else if (animationStep > 0 && Rt > 0 && Rt < entityTree->nRevisions) // Moving forwards
	{
		p.x = (1-animationStep)*b->normalizedProjectionPoints[Rt-1].x
					+ animationStep  *b->normalizedProjectionPoints[Rt].x;
		p.y = (1-animationStep)*b->normalizedProjectionPoints[Rt-1].y
					+ animationStep  *b->normalizedProjectionPoints[Rt].y;
	}
	else if (animationStep < 0 && Rt < entityTree->nRevisions) // Moving backwards
	{
		animationStep*=-1;
		p.x = (1-animationStep)*b->normalizedProjectionPoints[Rt+1].x
					+ animationStep  *b->normalizedProjectionPoints[Rt].x;
		p.y = (1-animationStep)*b->normalizedProjectionPoints[Rt+1].y
					+ animationStep  *b->normalizedProjectionPoints[Rt].y;
	}
	return p;
}
