#include "../include/ProjectionCanvas.h"

ProjectionCanvas::ProjectionCanvas(Point tl, Point br, EntityData *et)
{
	setSize(tl, br);
	entityData = et;
	double shortEdge = min(currentHeight, currentWidth);
	entityData->normalizeProjection(shortEdge - 50);
}

// Mark entities as selected from projection pane interaction
void ProjectionCanvas::getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click, bool ctrlDown)
{
	Entity *closest = NULL;
	double smallerDist = FLT_MAX;

	if (click && !ctrlDown)
		entityData->selected.clear();

	entityData->hovered = NULL;

	xRatio = currentWidth/initialWidth;
	yRatio = currentHeight/initialHeight;
	double minRatio = min(xRatio, yRatio);

	for (auto entity : entityData->entities)
	{
		double bx = entity->normalizedProjectionPoints[Rt].x * minRatio;
		double by = entity->normalizedProjectionPoints[Rt].y * minRatio;
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
					closest = entity;
				}
				else
				{
					smallerDist = dist;
					entityData->hovered = entity;
				}
			}
		}
		else if (bx > drag[0] && bx < drag[2] && by > drag[1] && by < drag[3]) // If inside selection box
		{
			if ((std::find(entityData->selected.begin(), entityData->selected.end(),entity))!= entityData->selected.end())
				entityData->selected.erase(std::find(entityData->selected.begin(), entityData->selected.end(),entity));
			else
				entityData->selected.push_back(entity);
		}
	}
	if (click == 1 && closest != NULL)
	{
		if ((std::find(entityData->selected.begin(), entityData->selected.end(),closest))!=entityData->selected.end())
			entityData->selected.erase(std::find(entityData->selected.begin(), entityData->selected.end(),closest));
		else
			entityData->selected.push_back(closest);
	}
}

// Draw scaled Canvas
void ProjectionCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3d(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	// Scale initial aspect ratio by new
	glPushMatrix();
	glTranslated(xOff, yOff, 0.0);

	xRatio = currentWidth/initialWidth;
	yRatio = currentHeight/initialHeight;
	double minRatio = min(xRatio, yRatio);
	glScaled(minRatio, minRatio, 1.0);

	glEnable(GL_LINE_SMOOTH);

	unsigned rMetric = (unsigned) controller.radiusMetricIndex;

	// Draw halos
	if (controller.halo)
	{
		for (auto entity : entityData->entities)
		{
			if (entity->showHalo)
			{
				Point p = getPoint(entity, Rt, animationStep);
				double radius = entity->normalizedData[Rt][rMetric];
				drawHalo(p.x, p.y, radius, animationStep);
			}
		}
	}

	// Draw selected entities
	for (auto selected : entityData->selected)
	{
		Point p = getPoint(selected, Rt, animationStep);
		double value = selected->data[Rt][rMetric];
		double radius = selected->normalizedData[Rt][rMetric];
		double delta = (Rt > 1) ? (value - selected->data[Rt-1][rMetric])/value: 0;
		drawEntity(p.x, p.y, radius, delta, colorSelection, 1);
	}

	// Draw hovered entities
	if (entityData->hovered != NULL)
	{
		if (entityData->hovered->isEntity())
		{
			Entity *hovered = (Entity*) entityData->hovered;
			Point p = getPoint(hovered, Rt, animationStep);
			double value = hovered->data[Rt][rMetric];
			double radius = hovered->normalizedData[Rt][rMetric];
			double delta = (Rt > 1) ? (value - hovered->data[Rt-1][rMetric])/value: 0;
			drawEntity(p.x, p.y, radius, delta, colorHover, 1);
		}
		else if (entityData->hovered->isPackage())
		{
			// Recursive lambda function to find all entities belonging to a package
			Package *hovered = (Package*) entityData->hovered;
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
						double radius = e.normalizedData[Rt][rMetric];
						double delta = (Rt > 1) ? (value - e.data[Rt-1][rMetric])/value: 0;
						drawEntity(p.x, p.y, radius, delta, colorHover, 1);
					}
				}
			};
			f(hovered);
		}
	}

	// Draw all entities
	for (auto b : entityData->entities)
	{
		Point p = getPoint(b, Rt, animationStep);
		Color c = getColor(controller.colormapIndex, b, Rt);
		double radius = b->normalizedData[Rt][rMetric];
		double delta = (Rt > 1) ? (b->data[Rt][rMetric] - b->data[Rt-1][rMetric])/b->data[Rt][rMetric]: 0;
		drawEntity(p.x, p.y, radius, delta, c, 0);
	}
	glDisable(GL_LINE_SMOOTH);
	glPopMatrix();
}

// Draw shadow
void ProjectionCanvas::drawHalo(double x, double y, double radius, double animationStep)
{
	int triangleAmount = 360; //# of triangles/degrees used to draw circle

	radius = (radius*20 + 7)*2; // Ugly as frick

	// Create the opacity curve like a triangle
	animationStep = (animationStep < 0)? animationStep*(-1) : animationStep;
	double opacity = (animationStep < 0.5)? 2*animationStep : 1 - (animationStep-0.5)*2;

	double radians = 2.0f * PI;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLE_FAN);
	glColor4d(0,0,0,opacity);
	glVertex3d(x, y, 0);
	glColor4d(0,0,0,0);
	for (int i = 0; i <= triangleAmount;i++)
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();
	glDisable(GL_BLEND);

}


// Draw circles
void ProjectionCanvas::drawEntity(double x, double y, double radius, double delta, Color c, int action)
{
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

	double radians = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(c.R,c.G,c.B);
	glVertex3d(x, y, 0);
	for (int i = 0; i <= triangleAmount; ++i)
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3d(0,0,0);
	for (int i = 0; i <= triangleAmount; ++i)
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
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

	double radians = 2.0f * PI;
	if (controller.deltaPie)
		radians *= (1 - fabs(delta)); // Take only a fraction of the 360 degrees

	// Old indian trick to rotate a circle around it's center
	glPushMatrix();
	glColor3d(c.R,c.G,c.B);
	double rotation = 0;
	glTranslated(x, y ,0); // Translate vector to the object's position

	// Define removed slice accordingly to delta value
	if (delta > 0)
		rotation = -90 + delta*180;
	else
		rotation = 90 - delta*180;

	glRotated(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

	glTranslated(-x, -y ,0); // Translate to normal origin

	// Draw circle with missing slice
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(x, y, 0);
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();

	// Draw stroke
	glBegin(GL_LINE_STRIP);
	glColor3d(0,0,0);
	glVertex3d(x, y, 0);
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glVertex3d(x, y, 0);
	glEnd();

	glPopMatrix();

	// Draw missing slice
	if (delta > 0)
		radius *= 1.2;
	else
		radius = 0;

	radians = 2*PI - radians;

	glPushMatrix();
	glColor3d(c.R,c.G,c.B);
	glTranslated(x, y ,0); // Translate vector to the object's position

	// Define removed slice accordingly to delta value
	if (delta > 0)
		rotation = -90 - delta*180;
	else
		rotation = 90 + delta*180;

	glRotated(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

	glTranslated(-x, -y ,0); // Translate to normal origin

	// Draw missing slice
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(x, y, 0);
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(x, y ,0); // Translate vector to the object's position

	// Define removed slice accordingly to delta value
	if (delta > 0)
		rotation = -90 - delta*180;
	else
		rotation = 90 + delta*180;

	glRotated(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

	glTranslated(-x, -y ,0); // Translate to normal origin

	// Draw missing slice
	glBegin(GL_LINE_STRIP);
	glLineWidth(2.0);
	glVertex3d(x, y, 0);
	for (int i = 0; i <= triangleAmount;i++) // Draw x percent of circle
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glVertex3d(x, y, 0);
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
	else if (animationStep > 0 && Rt > 0 && Rt < entityData->nRevisions) // Moving forwards
	{
		p.x = (1-animationStep)*b->normalizedProjectionPoints[Rt-1].x
					+ animationStep  *b->normalizedProjectionPoints[Rt].x;
		p.y = (1-animationStep)*b->normalizedProjectionPoints[Rt-1].y
					+ animationStep  *b->normalizedProjectionPoints[Rt].y;
	}
	else if (animationStep < 0 && Rt < entityData->nRevisions) // Moving backwards
	{
		animationStep*=-1;
		p.x = (1-animationStep)*b->normalizedProjectionPoints[Rt+1].x
					+ animationStep  *b->normalizedProjectionPoints[Rt].x;
		p.y = (1-animationStep)*b->normalizedProjectionPoints[Rt+1].y
					+ animationStep  *b->normalizedProjectionPoints[Rt].y;
	}
	return p;
}
