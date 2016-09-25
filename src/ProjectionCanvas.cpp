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
	double smallerDist = FLT_MAX;

	if (click && !ctrlDown)
		entityTree->selected.clear();

	entityTree->hovered = NULL;

	for (auto b : entityTree->entities)
	{
		double bx = b->normalizedProjectionPoints[Rt].x * xRatio;
		double by = b->normalizedProjectionPoints[Rt].y * yRatio;
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
					closest = b;
				}
				else
				{
					smallerDist = dist;
					entityTree->hovered = b;
				}
			}
		}
		else if (bx > drag[0] && bx < drag[2] && by > drag[1] && by < drag[3]) // If inside selection box
		{
			if ((std::find(entityTree->selected.begin(), entityTree->selected.end(),b))!= entityTree->selected.end())
				entityTree->selected.erase(std::find(entityTree->selected.begin(), entityTree->selected.end(),b));
			else
				entityTree->selected.push_back(b);
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

	int rMetric = entityTree->getRadiusMetric();
	double rMin = entityTree->getRMMin();
	double rMax = entityTree->getRMMax();

	// Draw halos
	if (controller.halo)
	{
		for (auto b : entityTree->entities)
		{
			if (b->showHalo)
			{
				Point p = getPoint(b, Rt, animationStep);
				double radius = (b->data[Rt][rMetric] - rMin) / (rMax - rMin);
				drawHalo(p.x, p.y, radius, animationStep);
			}
		}
	}

	// Draw selected entities
	for (auto b : entityTree->selected)
	{
		Point p = getPoint(b, Rt, animationStep);
		double value = b->data[Rt][rMetric];
		double radius = ((value) - rMin) / (rMax - rMin);
		double delta = (Rt > 1) ? (value - b->data[Rt-1][rMetric])/value: 0;
		drawEntity(p.x, p.y, radius, delta, colorSelection, 1);
	}

	// Draw hovered entities
	if (entityTree->hovered != NULL)
	{
		if (entityTree->hovered->isEntity())
		{
			Entity *hovered = (Entity*) entityTree->hovered;
			Point p = getPoint(hovered, Rt, animationStep);
			double value = hovered->data[Rt][rMetric];
			double radius = ((value) - rMin) / (rMax - rMin);
			double delta = (Rt > 1) ? (value - hovered->data[Rt-1][rMetric])/value: 0;
			drawEntity(p.x, p.y, radius, delta, colorHover, 1);
		}
		else if (entityTree->hovered->isPackage())
		{
			// Recursive lambda function to find all entities belonging to a package
			Package *hovered = (Package*) entityTree->hovered;
			function<void (Package*)> f;
			f = [&](Package* p)
			{
				if (p == NULL)
					return;
				else
				{
					for (auto c : p->childrenVector)
						f(&c);

					for (auto e : p->entityVector)
					{
						Point p = getPoint(&e, Rt, animationStep);
						double value = e.data[Rt][rMetric];
						double radius = (value - rMin) / (rMax - rMin);
						double delta = (Rt > 1) ? (value - e.data[Rt-1][rMetric])/value: 0;
						drawEntity(p.x, p.y, radius, delta, colorHover, 1);
					}
				}
			};
			f(hovered);
		}
	}

	// Draw all entities
	for (auto b : entityTree->entities)
	{
		Point p = getPoint(b, Rt, animationStep);
		Color c = getColor(controller.colormapIndex, b, Rt);
		double radius = (b->data[Rt][rMetric] - rMin) / (rMax - rMin);
		double delta = (Rt > 1) ? (b->data[Rt][rMetric] - b->data[Rt-1][rMetric])/b->data[Rt][rMetric]: 0;
		drawEntity(p.x, p.y, radius, delta, c, 0);
	}
	glDisable(GL_LINE_SMOOTH);
	glPopMatrix();
}

// Draw shadow
void ProjectionCanvas::drawHalo(double x, double y, double radius, double animationStep)
{
	x += 10;
	y += 10;
	int triangleAmount = 360; //# of triangles/degrees used to draw circle

	radius = (radius*20 + 7)*2; // Ugly as frick

	animationStep = (animationStep < 0)? animationStep*(-1) : animationStep;
	float opacity = (animationStep < 0.5)? 2*animationStep : 1 - (animationStep-0.5)*2;

	GLfloat radians = 2.0f * PI;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0,0,0,opacity);
	glVertex3f(x, y, 0);
	glColor4f(0,0,0,0);
	for (int i = 0; i <= triangleAmount;i++)
	{
		glVertex3f(x + (radius * cos(i * radians / triangleAmount)),
							 y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();
	glDisable(GL_BLEND);

}


// Draw circles
void ProjectionCanvas::drawEntity(double x, double y, double radius, double delta, Color c, int action)
{
	x+=10;
	y+=10;
	if (controller.deltaPie == 0 || fabs(delta) < 0.01) // 1% tolerance
		drawSolidEntity(x, y, radius, c, action);
	else
		drawPieEntity(x, y, radius, delta, c, action);
}

void ProjectionCanvas::drawSolidEntity(double x, double y, double radius, Color c, int action)
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


void ProjectionCanvas::drawPieEntity(double x, double y, double radius, double delta, Color c, int action)
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
