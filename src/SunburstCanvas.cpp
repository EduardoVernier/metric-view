#include "../include/SunburstCanvas.h"

SunburstCanvas::SunburstCanvas (Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
	initialWidth  = br.x - tl.x;
	initialHeight = br.y - tl.y;
}

void SunburstCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	double unitWidth = (2.0*PI)/(double)entityTree->entities.size();
	double currentTheta = 0.0;
	for (auto b : entityTree->sortedEntities)
	{
		drawSlice(b, Rt, currentTheta);

		if (b->isEntity())
			currentTheta += unitWidth;
	}
}


void SunburstCanvas::drawSlice(BaseEntity* b, unsigned Rt, double currentTheta)
{
	double x = xOff + initialWidth/2;
	double y = yOff + initialHeight/2;

	double innerDiameter = 50;
	double unitWidth = (2.0*PI)/(double)entityTree->entities.size();
	double r0 = b->getLevel()*50 + innerDiameter;
	double r1 = (b->getLevel()+1)*50 + innerDiameter;
	double theta0 = currentTheta;
	double theta1;
	if(b->isPackage())
		theta1 = currentTheta + ((Package*)b)->numberOfEntities*unitWidth;
	else
		theta1 = currentTheta + unitWidth;


	// Colored polygon
	Color c = getColor(controller.colormapIndex, b, Rt);
	glColor3f(c.R,c.G,c.B);
	glBegin(GL_QUADS);

	// Upper arch
	for (double t = theta0; t < theta1-0.01; t+=0.001)
	{
		glVertex3f(x + (r0 * cos(t)), y + (r0 * sin(t)), 0);
		glVertex3f(x + (r1 * cos(t)), y + (r1 * sin(t)), 0);
		glVertex3f(x + (r1 * cos(t+0.01)), y + (r1 * sin(t+0.01)), 0);
		glVertex3f(x + (r0 * cos(t+0.01)), y + (r0 * sin(t+0.01)), 0);
	}

	glEnd();


	// Line around it
	glBegin(GL_LINE_STRIP);
	glColor3f(0,0,0);

	glVertex3f(x + (r0 * cos(theta0)), y + (r0 * sin(theta0)), 0);

	// Upper arch
	for (double t = theta0; t <= theta1; t+=0.01)
		glVertex3f(x + (r1 * cos(t)), y + (r1 * sin(t)), 0);

	glVertex3f(x + (r0 * cos(theta1)), y + (r0 * sin(theta1)), 0);

	// Lower arch
	for (double t = theta1; t >= theta0; t-=0.01)
		glVertex3f(x + (r0 * cos(t)), y + (r0 * sin(t)), 0);

	glEnd();

}
