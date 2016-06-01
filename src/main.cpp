#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include "../include/renderer.h"
#include "../include/interaction.h"
#include "../include/EntityTree.h"
#include "../include/CsvParser.h"
#include "../include/Treemap.h"

using namespace std;

void initilizeVisualization(int argc, char **argv);
void buildEntityTree(int argc, char **argv, EntityTree *et);

unsigned winWidth = 1630, winHeight = 800;
int main_window;
Treemap *treemap; // Global singletons

int main(int argc, char **argv)
{
	initilizeVisualization(argc, argv);

	// Build entity tree from csv metric files
	EntityTree *entityTree = new EntityTree();
	buildEntityTree(argc, argv, entityTree);
	// Use entity tree to generate a squarified treemap
	treemap = new Treemap (entityTree, (double)(winWidth-30)/2, (double)winHeight-20);
	//treemap->getTree()->printTree();

	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}


void initilizeVisualization(int argc, char **argv)
{
	// Initialize Glut/Glui
	glutInit(&argc, argv);
 	glutInitWindowSize(winWidth, winHeight);
	main_window = glutCreateWindow("MetricView");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mousePassive);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


void buildEntityTree(int argc, char **argv, EntityTree *et)
{
	// Check if a string was inputed
	if (argc < 3)
	{
		cout << ("Usage: runner <datadir> <projectiondir>\n");
		exit(1);
	}

	CsvParser parser (et, argv[1], argv[2]);
}
