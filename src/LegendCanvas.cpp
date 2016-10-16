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
	glPushMatrix();
	glTranslated(xOff, yOff, 0);

	glColor3d(1,1,1);
	glRectd(0, 0, currentWidth, currentHeight);

	glPushMatrix();
	glScaled(ProjectionCanvas::getInstance().getMinRatio(), ProjectionCanvas::getInstance().getMinRatio(), 1.0);

	double radiusMaxMetricValue = entityData->maxMetricValue[controller.radiusMetricIndex];
	double radiusMinMetricValue = entityData->minMetricValue[controller.radiusMetricIndex];

	// Format and print min/max metric values
	stringstream stream;
	stream << fixed << setprecision(2) << radiusMinMetricValue;
	string min = stream.str();
	min.replace(min.find(".00"), 3, "");

	stream.str(""); // Clear stream
	stream << fixed << setprecision(2) << radiusMaxMetricValue;
	string max = stream.str();
	max.replace(max.find(".00"), 3, "");

	string range = min + " - " + max;

	glColor3d(0, 0, 0);
	glRasterPos2i(10, 35);
	const unsigned char* s = reinterpret_cast<const unsigned char *>(range.c_str());
	glutBitmapString(GLUT_BITMAP_9_BY_15, s);

	// Draw min max radius circles
	double x = 10, y = 80;
	int triangleAmount = 360; //# of triangles/degrees used to draw circle
	double radius = MIN_RADIUS;
	double radians = 2.0f * PI;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= triangleAmount; ++i)
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();

	x = currentWidth - MAX_RADIUS - 2;
	radius = MAX_RADIUS;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= triangleAmount; ++i)
	{
		glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
				   y + (radius * sin(i * radians / triangleAmount)), 0);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(20, currentHeight - 5, 0);
	glScalef(0.15f,-0.15f,0);
	glRotated(90, 0, 0, 1);

	string PTColormap = "Projection/Treemap Colormap - " + entityData->metricVector[controller.colorMetricIndex];
	s = reinterpret_cast<const unsigned char *>(PTColormap.c_str());
	glutStrokeString(GLUT_STROKE_ROMAN , s);
	glPopMatrix();

	glPushMatrix();
	glTranslated(70, currentHeight - 5, 0);
	glScalef(0.15f,-0.15f,0);
	glRotated(90, 0, 0, 1);
	string EvolColormap = "Evolution View Colormap - " + entityData->metricVector[controller.evolutionMetricIndex];
	s = reinterpret_cast<const unsigned char *>(EvolColormap.c_str());
	glutStrokeString(GLUT_STROKE_ROMAN , s);
	glPopMatrix();

	glPopMatrix();

}
