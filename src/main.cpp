#include <iostream>
#include <stdio.h>
//#include <GL/glut.h>
//#include <GL/glui.h>

//#include "../include/renderer.h"
//#include "../include/interaction.h"
#include "../include/EntityTree.h"
#include "../include/CsvParser.h"
#include "../include/Treemap.h"

using namespace std;

//void initilizeVisualization(int argc, char **argv);
EntityTree buildEntityTree(int argc, char **argv);

int main_window;

int main(int argc, char **argv)
{
	
	//initilizeVisualization(argc, argv);
	EntityTree entityTree = buildEntityTree(argc, argv);
	Treemap treemap (entityTree, 600, 450);

	//glutIdleFunc(idle);
	//glutMainLoop();
	return 0;
}

/*
void initilizeVisualization(int argc, char **argv)
{
	// Initialize Glut/Glui
	glutInit(&argc, argv);
 	glutInitWindowSize(500,500);
	main_window = glutCreateWindow("MetricView");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
}
*/


EntityTree buildEntityTree(int argc, char **argv)
{
	// Check if a string was inputed
	if (argc < 2)
	{
		cout << ("Usage: runner <dirname>\n");
		exit(1);
	}

	CsvParser parser (argv[1]);
	return parser.getEntityTree();
}
