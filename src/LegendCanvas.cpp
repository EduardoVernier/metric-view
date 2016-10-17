#include <iomanip>
#include "../include/LegendCanvas.h"

LegendCanvas::LegendCanvas() {}

LegendCanvas& LegendCanvas::getInstance()
{
	static LegendCanvas instance {};
	return instance;
}

void LegendCanvas::init(Point tl, Point br, EntityData *ed)
{
	setSize(tl, br);
	entityData = ed;
}

void LegendCanvas::drawCanvas(unsigned Rt, double animationStep)
{
//	glPushMatrix();
//	glTranslated(xOff, yOff, 0);
//
//	glColor3d(1,1,1);
//	glRectd(0, 0, currentWidth, currentHeight);
//
//	glPushMatrix();
//	glScaled(ProjectionCanvas::getInstance().getMinRatio(), ProjectionCanvas::getInstance().getMinRatio(), 1.0);
//
//
//	// Write colorbar labels
//	glPushMatrix();
//	glTranslated(20, currentHeight - 5, 0);
//	glScalef(0.15f,-0.15f,0);
//	glRotated(90, 0, 0, 1);
//
//	string PTColormap = "Projection/Treemap Colormap - " + entityData->metricVector[controller.colorMetricIndex];
//	s = reinterpret_cast<const unsigned char *>(PTColormap.c_str());
//	glutStrokeString(GLUT_STROKE_ROMAN , s);
//	glPopMatrix();
//
//	glPushMatrix();
//	glTranslated(70, currentHeight - 5, 0);
//	glScalef(0.15f,-0.15f,0);
//	glRotated(90, 0, 0, 1);
//	string EvolColormap = "Evolution View Colormap - " + entityData->metricVector[controller.evolutionMetricIndex];
//	s = reinterpret_cast<const unsigned char *>(EvolColormap.c_str());
//	glutStrokeString(GLUT_STROKE_ROMAN , s);
//	glPopMatrix();
//
//	double colorbarLength = currentHeight - 150;
//	glColor3d(1, 0,0);
//	glRectd(30, currentHeight - 5, 50, currentHeight - 5 - colorbarLength);
//
//	glRectd(80, currentHeight - 5, 100, currentHeight - 5 - colorbarLength);
//
//
//	glPopMatrix();

}
