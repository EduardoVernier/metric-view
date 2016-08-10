#include "../include/TreemapCanvas.h"

TreemapCanvas::TreemapCanvas (Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
	initialWidth  = br.x - tl.x;
	initialHeight = br.y - tl.y;
}

// Fill vector of pointers of the selected entities
// For some strange reason, the entityTree member is always NULL in this method, so I'm passing it's pointer as arg
void TreemapCanvas::getEntitiesByPositionOnTreemap(int *drag, unsigned click, unsigned ctrlDown)
{
	if (click && !ctrlDown)
		entityTree->selected.clear();

	for (vector<BaseEntity*>::iterator b = entityTree->sortedEntities.begin(); b != entityTree->sortedEntities.end(); ++b)
	{
		if ((*b)->isPackage() == 0)
		{
			if ((*b)->getCoord(3) * yRatio < drag[1])
				continue; // b too high
			else if ((*b)->getCoord(1) * yRatio > drag[3])
				continue; // svg too low
			else if ((*b)->getCoord(2) * xRatio < drag[0])
				continue; // svg too far left
			else if ((*b)->getCoord(0) * xRatio > drag[2])
				continue; // svg too far right
			else
			{
				if (click)
				{
					if ((std::find(entityTree->selected.begin(), entityTree->selected.end(),(Entity*)*b))!= entityTree->selected.end())
						entityTree->selected.erase(std::find(entityTree->selected.begin(),entityTree-> selected.end(),(Entity*)*b));
					else
						entityTree->selected.push_back((Entity*)*b);
				}
				else
					entityTree->hovered = (Entity*)*b;
			}
		}
	}
}

// First draw elements and then package borders
void TreemapCanvas::drawCanvas(unsigned Rt)
{
	vector<BaseEntity*> items = entityTree->sortedEntities;

	// Scale initial aspect ratio by new
	glPushMatrix();
	xRatio = double(currentWidth)/double(initialWidth);
	yRatio = double(currentHeight)/double(initialHeight);
	glScaled(xRatio, yRatio, 1);

	// Draw treemap squares
	for (vector<BaseEntity*>::iterator it = entityTree->sortedEntities.begin(); it != entityTree->sortedEntities.end(); ++it)
	{
		if ((*it)->isPackage() == 0)
		{
			drawEntity(*it, Rt);
		}
	}

	// Draw Package borders
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
	labelCells();

	glPopMatrix();
}

void TreemapCanvas::drawEntity(BaseEntity *e, unsigned Rt)
{
	double padding = 0.5;
	double x0 = e->getCoord(0)+ padding + xOff;
	double y0 = e->getCoord(1)+ padding + yOff;
	double x1 = e->getCoord(2)- padding + xOff;
	double y1 = e->getCoord(3)- padding + yOff;

	int cMetric = entityTree->getColorMetric();
	float cMin = entityTree->getCMMin();
	float cMax = entityTree->getCMMax();

	float value = ((Entity*)e)->data[Rt][cMetric];
	float normCValue = (value - cMin) / (cMax - cMin);

	// Dynamic treemap experiment
	if (dynamicTreemap)
	{
		double ratio = sqrt(((Entity*)e)->data[Rt][21]/((Entity*)e)->getScore());
		x0 = x0 + (((1-ratio)*(x1-x0))/2);
		x1 = x0 + ratio*(x1-x0);
		y0 = y0 + (((1-ratio)*(y1-y0))/2);
		y1 = y0 + ratio*(y1-y0);
	}


	// Generate color based on colormap index value
	Color c (1,1,1);
	switch (colormapIndex)
	{
		case 0:
			c = sequentialColormap(normCValue);
			break;
		case 1:
			c = qualitativeColormap(((Entity*)e)->firstLevelId);
			break;
		case 2:
			if (Rt > 0)
			{
				float pValue = ((Entity*)e)->data[Rt-1][cMetric];
				float pNormCValue = ((pValue - cMin) / (cMax - cMin));
				c = divergentColormap(normCValue-pNormCValue);
			}
			break;
	}

	glColor3f(c.R, c.G, c.B);
	glRectd(x0,y0,x1,y1);
}

void TreemapCanvas::drawPackage(BaseEntity *e)
{
	double x0 = e->getCoord(0) + xOff;
	double y0 = e->getCoord(1) + yOff;
	double x1 = e->getCoord(2) + xOff;
	double y1 = e->getCoord(3) + yOff;

	glLineWidth(3.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0,y0-1);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1+1);
	glVertex2d(x0,y1);
	glVertex2d(x1+1,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y0-1);
	glVertex2d(x1,y0);
	glVertex2d(x0,y0);
	glEnd();
}

void TreemapCanvas::drawHovered(Entity *e)
{
	if (e == NULL) return;

	double padding = 3.0;
	double x0 = e->getCoord(0)+ padding + xOff;
	double y0 = e->getCoord(1)+ padding + yOff;
	double x1 = e->getCoord(2)- padding + xOff;
	double y1 = e->getCoord(3)- padding + yOff;

	glLineWidth(6.0f);
	glColor3f(colorHover.R, colorHover.G, colorHover.B);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0,y0-padding);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1+padding);
	glVertex2d(x0,y1);
	glVertex2d(x1+padding,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y0-padding);
	glVertex2d(x1,y0);
	glVertex2d(x0,y0);
	glEnd();

	padding = 0.0;
	x0 = e->getCoord(0)+ padding + xOff;
	y0 = e->getCoord(1)+ padding + yOff;
	x1 = e->getCoord(2)- padding + xOff;
	y1 = e->getCoord(3)- padding + yOff;

	glLineWidth(1.0f);
	glColor3f(1,1,1);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y0);
	glVertex2d(x0,y0);
	glEnd();
}

void TreemapCanvas::drawSelected(Entity *e)
{
	int showHierarchy = 0;
	if (showHierarchy)
	{
		;
	}
	else
	{
		double padding = 2.0;
		double x0 = e->getCoord(0)+ padding + xOff;
		double y0 = e->getCoord(1)+ padding + yOff;
		double x1 = e->getCoord(2)- padding + xOff;
		double y1 = e->getCoord(3)- padding + yOff;

		glLineWidth(4.0f);
		glColor3f(colorSelection.R, colorSelection.G, colorSelection.B);
		glBegin(GL_LINE_STRIP);
		glVertex2d(x0,y0-padding);
		glVertex2d(x0,y0);
		glVertex2d(x0,y1+padding);
		glVertex2d(x0,y1);
		glVertex2d(x1+padding,y1);
		glVertex2d(x1,y1);
		glVertex2d(x1,y0-padding);
		glVertex2d(x1,y0);
		glVertex2d(x0,y0);
		glEnd();

		padding = 0.0;
		x0 = e->getCoord(0)+ padding + xOff;
		y0 = e->getCoord(1)+ padding + yOff;
		x1 = e->getCoord(2)- padding + xOff;
		y1 = e->getCoord(3)- padding + yOff;

		glLineWidth(1.0f);
		glColor3f(1,1,1);
		glBegin(GL_LINE_STRIP);
		glVertex2d(x0,y0);
		glVertex2d(x0,y1);
		glVertex2d(x1,y1);
		glVertex2d(x1,y0);
		glVertex2d(x0,y0);
		glEnd();
	}

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


void TreemapCanvas::labelCells()
{
	for (vector<BaseEntity*>::iterator it = entityTree->sortedEntities.begin(); it != entityTree->sortedEntities.end(); ++it)
	{
		if ((*it)->isPackage() == 0)
		{
			double x0 = (*it)->getCoord(0) + xOff;
			double y0 = (*it)->getCoord(1) + yOff;
			double x1 = (*it)->getCoord(2) + xOff;
			double y1 = (*it)->getCoord(3) + yOff;

			// Disregard very small cells
			if ((x1-x0 > y1-y0 && x1-x0 < 50) || y1-y0 < 15)
				continue;
			if ((y1-y0 > x1-x0 && y1-y0 < 50) || x1-x0 < 15)
				continue;


			// Initialize transformation matrix
			glPushMatrix();
			glColor3f(0, 0, 0);

			// Extract name and cast it to a glut accepted type
			string str = (*it)->getName();
			const unsigned char* s = reinterpret_cast<const unsigned char *>(str.c_str());
			unsigned width = 0;

			// Measure the width of text
			for (unsigned i = 0; i < str.size(); ++i)
			{
				width += glutStrokeWidth(GLUT_STROKE_ROMAN , s[i]);
			}

			// Test for horizontal fit
			if (x1-x0 > width*0.12 + 200*0.12)
			{
				glTranslatef(x0+3, y0+15, 0);
				glScalef(0.12f,-0.1f,0);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glEnable(GL_LINE_SMOOTH);
				glLineWidth(1.0);

				glutStrokeString(GLUT_STROKE_ROMAN , s);

				glDisable(GL_BLEND);
				glDisable(GL_LINE_SMOOTH);
				glPopMatrix();
			}
			else if (y1-y0 > width*0.12 + 200*0.12) // Test for vertical fit
			{
				glTranslatef(x0+13, y1-3, 0);
				glScalef(0.1f,-0.12f,0);
				glRotated(90, 0, 0, 1);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glEnable(GL_LINE_SMOOTH);
				glLineWidth(1.0);

				glutStrokeString(GLUT_STROKE_ROMAN , s);

				glDisable(GL_BLEND);
				glDisable(GL_LINE_SMOOTH);
				glPopMatrix();
			}
			else if (x1-x0 > y1-y0) // Fit all that's possible on horizontal plane
			{
				glTranslatef(x0+3, y0+15, 0);
				glScalef(0.12f,-0.1f,0);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glEnable(GL_LINE_SMOOTH);
				glLineWidth(1.0);

				int sub_width = 0, sub_index = 0;
				while (sub_width*0.12 + 160*0.12 < x1-x0)
				{
					sub_width += glutStrokeWidth(GLUT_STROKE_ROMAN , s[sub_index]);
					glutStrokeCharacter(GLUT_STROKE_ROMAN , s[sub_index]);
					sub_index++;
				}
				glutStrokeCharacter(GLUT_STROKE_ROMAN , '.');
				glutStrokeCharacter(GLUT_STROKE_ROMAN , '.');

				glDisable(GL_BLEND);
				glDisable(GL_LINE_SMOOTH);
				glPopMatrix();
			}
			else if (y1-y0 > x1-x0) // Fit all that's possible on vertical plane
			{
				glTranslatef(x0+13, y1-3, 0);
				glScalef(0.1f,-0.12f,0);
				glRotated(90, 0, 0, 1);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glEnable(GL_LINE_SMOOTH);
				glLineWidth(1.0);

				int sub_width = 0, sub_index = 0;
				while (sub_width*0.12 + 160*0.12 < y1-y0)
				{
					sub_width += glutStrokeWidth(GLUT_STROKE_ROMAN , s[sub_index]);
					glutStrokeCharacter(GLUT_STROKE_ROMAN , s[sub_index]);
					sub_index++;
				}
				glutStrokeCharacter(GLUT_STROKE_ROMAN , '.');
				glutStrokeCharacter(GLUT_STROKE_ROMAN , '.');

				glDisable(GL_BLEND);
				glDisable(GL_LINE_SMOOTH);
				glPopMatrix();
			}
		}
	}
}
