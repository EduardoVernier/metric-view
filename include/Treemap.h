#pragma once

#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>

#include "BaseEntity.h"
#include "EntityData.h"

using namespace std;

class Treemap {

public:
    //
    class Container {
    public:
        Container(double _width, double _height, double _xOffset, double _yOffset);
        Container cutArea(double area);
        void saveCoordinates(vector<BaseEntity *> *row, double scoreSum); // Save coordinates INTO OBJECTS

        double width, height, xOffset, yOffset, shortestEdge;
    };
    Treemap(EntityData *ed, double _width, double _height);
    void updateSize(double _width, double _height);
    EntityData *getTree() { return entityData; };

private:
    void treemapMultidimensional(vector<BaseEntity *> *data, double width, double height, double xOffset, double yOffset);
    void treemapSingledimensional(vector<BaseEntity *> *data, double width, double height, double xOffset, double yOffset);
    void squarify(vector<BaseEntity *> *data, vector<BaseEntity *> *currentRow, Container container);
    void normalize(vector<BaseEntity *> *data, int area);
    int sumScores(vector<BaseEntity *> *data);
    double sumNormalizedScores(vector<BaseEntity *> *data);
    int improvesRatio(vector<BaseEntity *> *currentRow, double nextEntity, int length);

    EntityData *entityData;
    double width, height;
};
