#include "../include/TreemapCanvas.h"

TreemapCanvas::TreemapCanvas (Point tl, Point br, EntityData *ed)
{
	setSize(tl, br);
	entityData = ed;
}

// Fill vector of pointers of the selected entities
// For some strange reason, the entityData member is always NULL in this method, so I'm passing it's pointer as arg
void TreemapCanvas::getEntitiesByPositionOnTreemap(int *drag, unsigned click, bool ctrlDown)
{
	if (click && !ctrlDown)
		entityData->selected.clear();

	for (vector<BaseEntity*>::iterator b = entityData->sortedBaseEntities.begin(); b != entityData->sortedBaseEntities.end(); ++b)
	{
		if ((*b)->isEntity())
		{
			if ((*b)->getCoord(3) * yRatio < drag[1])	continue; // b too high
			else if ((*b)->getCoord(1) * yRatio > drag[3]) continue; // svg too low
			else if ((*b)->getCoord(2) * xRatio < drag[0]) continue; // svg too far left
			else if ((*b)->getCoord(0) * xRatio > drag[2]) continue; // svg too far right
			else
			{
				if (click)
				{
					if ((std::find(entityData->selected.begin(), entityData->selected.end(),(Entity*)*b))!= entityData->selected.end())
						entityData->selected.erase(std::find(entityData->selected.begin(),entityData-> selected.end(),(Entity*)*b));
					else
						entityData->selected.push_back((Entity*)*b);
				}
				else
					entityData->hovered = (Entity*)*b;
			}
		}
	}
}

// First draw elements and then package borders
void TreemapCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	// Scale initial aspect ratio by new
	glPushMatrix();
	glTranslated(xOff, yOff, 0.0);
	xRatio = currentWidth/initialWidth;
	yRatio = currentHeight/initialHeight;
	glScaled(xRatio, yRatio, 1.0);

	// Draw treemap class entities
	for (vector<Entity*>::iterator it = entityData->entities.begin(); it != entityData->entities.end(); ++it)
	{
		drawEntity(*it, Rt, animationStep);
	}

	// Draw Package borders
	for (vector<BaseEntity*>::iterator it = entityData->sortedBaseEntities.begin(); it != entityData->sortedBaseEntities.end(); ++it)
	{
		if ((*it)->isPackage())
		{
			drawPackageFirstLayer(*it);
		}
	}
	for (vector<BaseEntity*>::iterator it = entityData->sortedBaseEntities.begin(); it != entityData->sortedBaseEntities.end(); ++it)
	{
		if ((*it)->isPackage())
		{
			drawPackageSecondLayer(*it);
		}
	}

	for (vector<Entity*>::iterator it = entityData->selected.begin(); it != entityData->selected.end(); ++it)
	{
		drawSelected(*it);
	}
	drawHovered(entityData->hovered);
	labelCells();

	glPopMatrix();
}

// Draw a given entity considering the animation
void TreemapCanvas::drawEntity(Entity *e, unsigned Rt, double animationStep)
{
	double coords[4];
	computeRectangleSize(coords, e, Rt, animationStep);

	// Draw colored rect
	Color c = getColor(controller.colormapIndex, e, Rt);
	glColor3d(c.R, c.G, c.B);
	glRectd(coords[0],coords[1],coords[2],coords[3]);

	// Draw darker border
	double x0 = coords[0];
	double y0 = coords[1];
	double x1 = coords[2];
	double y1 = coords[3];
	glLineWidth(0.1f);
	glColor3d(c.R*0.6, c.G*0.6, c.B*0.6);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0,y0);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1);
	glVertex2d(x0,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y0);
	glVertex2d(x1,y0);
	glVertex2d(x0,y0);
	glEnd();

	if (controller.halo && e->showHalo) drawHalo(coords, animationStep);

	// Draw line around full rectangle
	x0 = e->getCoord(0);
	y0 = e->getCoord(1);
	x1 = e->getCoord(2);
	y1 = e->getCoord(3);
	glLineWidth(0.1f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0,y0);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1);
	glVertex2d(x0,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y0);
	glVertex2d(x1,y0);
	glVertex2d(x0,y0);
	glEnd();
}

void TreemapCanvas::drawHalo(const double *coords, double animationStep)
{
	if (animationStep != 0.0)
	{
		double x0 = coords[0];
		double y0 = coords[1];
		double x1 = coords[2];
		double y1 = coords[3];
		double xc = (x1+x0)/2.0;
		double yc = (y1+y0)/2.0;

		animationStep = (animationStep < 0)? animationStep*(-1) : animationStep;
		double opacity = (animationStep < 0.5)? 2*animationStep : 1 - (animationStep-0.5)*2;

		glEnable(GL_BLEND);
		glEnable(GL_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glBegin(GL_TRIANGLE_FAN);
		glColor4d(0,0,0,0);
		glVertex3d(xc, yc, 0);
		glColor4d(0.2,0.2,0.2,opacity);
		glVertex3d(x0, y0, 0);
		glVertex3d(x0, y1, 0);
		glVertex3d(x1, y1, 0);
		glVertex3d(x1, y0, 0);
		glVertex3d(x0, y0, 0);
		glEnd();
		glDisable(GL_BLEND);
	}

}

// Return an entitie's vertex coordinates on the treemap
void TreemapCanvas::computeRectangleSize(double *retCoords, Entity *e, unsigned Rt, double animationStep)
{
	double x0 = e->getCoord(0);
	double y0 = e->getCoord(1);
	double x1 = e->getCoord(2);
	double y1 = e->getCoord(3);
	double ratio = 1;

	// If we are dealing with a dynamic treemaps, we interpolate
	// the shrinking ratios for the animation to be smooth
	if (controller.dynamicTreemap)
	{
		ratio = sqrt(e->data[Rt][21]/e->getScore());
		double prevRatio = sqrt(e->data[Rt][21]/e->getScore());
		if (animationStep > 0 && Rt > 0 && Rt < entityData->nRevisions)
		{
			prevRatio = sqrt(e->data[Rt-1][21]/e->getScore());
		}
		else if (animationStep < 0 && Rt < entityData->nRevisions)
		{
			animationStep*=-1;
			prevRatio = sqrt(e->data[Rt+1][21]/e->getScore());
		}
		ratio = (1-animationStep)*prevRatio + animationStep*ratio;
	}

	retCoords[0] = x0 + ((1-ratio)*(x1-x0)/2);
	retCoords[1] = y0 + ((1-ratio)*(y1-y0)/2);
	retCoords[2] = x1 - ((1-ratio)*(x1-x0)/2);
	retCoords[3] = y1 - ((1-ratio)*(y1-y0)/2);
}

void TreemapCanvas::drawPackageFirstLayer(BaseEntity *e)
{
	double x0 = e->getCoord(0);
	double y0 = e->getCoord(1);
	double x1 = e->getCoord(2);
	double y1 = e->getCoord(3);

	glLineWidth(5.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0,y0);
	glVertex2d(x0,y1);
	glVertex2d(x1,y1);
	glVertex2d(x1,y0);
	glVertex2d(x0,y0);
	glEnd();
	glLineWidth(1.0f);
}

void TreemapCanvas::drawPackageSecondLayer(BaseEntity *e)
{
	double x0 = e->getCoord(0);
	double y0 = e->getCoord(1);
	double x1 = e->getCoord(2);
	double y1 = e->getCoord(3);

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
	glLineWidth(1.0f);
}


void TreemapCanvas::drawHovered(BaseEntity *e)
{
	if (e == NULL) return;

	double padding = 3.0;
	double x0 = e->getCoord(0)+ padding;
	double y0 = e->getCoord(1)+ padding;
	double x1 = e->getCoord(2)- padding;
	double y1 = e->getCoord(3)- padding;

	glLineWidth(6.0f);
	glColor3d(colorHover.R, colorHover.G, colorHover.B);
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

	x0 = e->getCoord(0);
	y0 = e->getCoord(1);
	x1 = e->getCoord(2);
	y1 = e->getCoord(3);

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
	double padding = 2.0;
	double x0 = e->getCoord(0) + padding;
	double y0 = e->getCoord(1) + padding;
	double x1 = e->getCoord(2) - padding;
	double y1 = e->getCoord(3) - padding;
	glLineWidth(4.0f);
	glColor3d(colorSelection.R, colorSelection.G, colorSelection.B);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0, y0 - padding);
	glVertex2d(x0, y0);
	glVertex2d(x0, y1 + padding);
	glVertex2d(x0, y1);
	glVertex2d(x1 + padding, y1);
	glVertex2d(x1, y1);
	glVertex2d(x1, y0 - padding);
	glVertex2d(x1, y0);
	glVertex2d(x0, y0);
	glEnd();

	x0 = e->getCoord(0);
	y0 = e->getCoord(1);
	x1 = e->getCoord(2);
	y1 = e->getCoord(3);
	glLineWidth(1.0f);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x0, y0);
	glVertex2d(x0, y1);
	glVertex2d(x1, y1);
	glVertex2d(x1, y0);
	glVertex2d(x0, y0);
	glEnd();

}

void TreemapCanvas::labelCells()
{
	for (vector<BaseEntity*>::iterator it = entityData->sortedBaseEntities.begin(); it != entityData->sortedBaseEntities.end(); ++it)
	{
		if ((*it)->isEntity())
		{
			double x0 = (*it)->getCoord(0);
			double y0 = (*it)->getCoord(1);
			double x1 = (*it)->getCoord(2);
			double y1 = (*it)->getCoord(3);

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
				glTranslated(x0+3, y0+15, 0);
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
				glTranslated(x0+13, y1-3, 0);
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
				glTranslated(x0+3, y0+15, 0);
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
				glTranslated(x0+13, y1-3, 0);
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
