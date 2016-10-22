#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include "../include/renderer.h"
#include "../include/interaction.h"
#include "../include/EntityData.h"
#include "../include/CsvParser.h"
#include "../include/Treemap.h"
#include "../include/Controller.h"

using namespace std;

void initializeVisualization(int argc, char **argv);
void buildEntityData(int argc, char **argv, EntityData *ed);
void configureGL();

unsigned Rt = 0; // Current revision
EntityData *entityData = new EntityData();
Controller controller;

int main(int argc, char **argv) {

    controller = Controller();

    initializeVisualization(argc, argv);

    // Build entity tree from csv metric files
    buildEntityData(argc, argv, entityData);
    // Use entity tree to generate a squarified treemap
    Treemap treemap(entityData, (double) (controller.winWidth - 30) / 2, (double) controller.winHeight - 20);

    initializeUI();
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}


void initializeVisualization(int argc, char **argv) {
    // Initialize Glut/Glui
    glutInit(&argc, argv);
    glutInitWindowSize(controller.winWidth, controller.winHeight);
    controller.mainWindow = glutCreateWindow("MetricView");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardMod);
    glutSpecialUpFunc(keyboardMod);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mousePassive);
    glutMotionFunc(mouseUpdate);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    configureGL();
}

void configureGL() {

    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void buildEntityData(int argc, char **argv, EntityData *ed) {
    // Check if a string was inputed
    if (argc < 3) {
        cout << ("Usage: runner <datadir> <projectiondir>\n");
        exit(1);
    }

    CsvParser parser(ed, argv[1], argv[2]);
}

