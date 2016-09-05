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
	glColor3f(1.0f, 1.0f, 0.1f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	double innerDiameter = 50;
	double unit_width = (2.0*PI)/(double)entityTree->entities.size();
	double currentTheta = 0.0;
	for (auto b : entityTree->sortedEntities)
	{
		double r0 = b->getLevel()*50 + innerDiameter;
		double r1 = (b->getLevel()+1)*50 + innerDiameter;
		double theta0 = currentTheta;
		double theta1;
		if(b->isPackage())
			theta1 = currentTheta + ((Package*)b)->numberOfEntities*unit_width;
		else
		{
			theta1 = currentTheta + unit_width;
			currentTheta = theta1;
		}

		drawSlice(Rt, r0, theta0, r1, theta1);
	}
}


void SunburstCanvas::drawSlice(unsigned Rt, double r0, double theta0, double r1, double theta1)
{
	double x = xOff + initialWidth/2;
	double y = yOff + initialHeight/2;

	glBegin(GL_POLYGON);
	glColor3f(1,0,0);

	glVertex3f(x + (r0 * cos(theta0)), y + (r0 * sin(theta0)), 0);

	// Upper arch
	for (double t = theta0; t <= theta1; t+=0.01)
		glVertex3f(x + (r1 * cos(t)), y + (r1 * sin(t)), 0);

	glVertex3f(x + (r0 * cos(theta1)), y + (r0 * sin(theta1)), 0);

	// Lower arch
	for (double t = theta1; t >= theta0; t-=0.01)
		glVertex3f(x + (r0 * cos(t)), y + (r0 * sin(t)), 0);

	glEnd();



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
