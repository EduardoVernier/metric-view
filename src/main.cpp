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

void initializeVisualization(int argc, char **argv);
void buildEntityTree(int argc, char **argv, EntityTree *et);

unsigned winWidth = 1920, winHeight = 1056;
int mainWindow;
unsigned Rt = 0; // Current revision
EntityTree *entityTree = new EntityTree();

int main(int argc, char **argv)
{
	initializeVisualization(argc, argv);

	// Build entity tree from csv metric files
	buildEntityTree(argc, argv, entityTree);
	// Use entity tree to generate a squarified treemap
	Treemap treemap (entityTree, (double)(winWidth-30)/2, (double)winHeight-20);

	initializeUI();
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}


void initializeVisualization(int argc, char **argv)
{
	// Initialize Glut/Glui
	glutInit(&argc, argv);
 	glutInitWindowSize(winWidth, winHeight);
	mainWindow = glutCreateWindow("MetricView");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardMod);
	glutSpecialUpFunc(keyboardMod);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mousePassive);
	glutMotionFunc(mouseUpdate);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDepthRange(0.0f, 1.0f);
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
