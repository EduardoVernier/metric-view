#include "../include/Canvas.h"

Canvas::Canvas(Point tl, Point br)
{
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(100, 0);
    glVertex2f(30, 0);
    glVertex2f(0, 50);
    glEnd();
}

void Canvas::setSize(Point tl, Point br)
{

}
