using namespace std;

#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include "../include/renderer.h"

int main_window;
int W = 1400, H = 1000;


int main(int argc, char **argv)
{
	printf("MetricView\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(W,H);
	main_window = glutCreateWindow("MetricView");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse_click);

	glutMainLoop();
	return 0;
}
