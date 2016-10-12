#pragma once

#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>

#include "BaseEntity.h"
#include "EntityData.h"
#include "Container.h"

using namespace std;

class Treemap
{
public:
	Treemap(EntityData *et, double _width, double _height);
	void updateSize(double _width, double _height);
	EntityData* getTree() { return entityData; };

private:
	void treemapMultidimensional(vector<BaseEntity*> *data, double width, double height, double xOffset, double yOffset);
	void treemapSingledimensional(vector<BaseEntity*> *data, double width, double height, double xOffset, double yOffset);
	void squarify(vector<BaseEntity*> *data, vector<BaseEntity*> *currentRow, Container container);
	void normalize(vector<BaseEntity*> *data, int area);
	int sumScores(vector<BaseEntity*> *data);
	double sumNormalizedScores(vector<BaseEntity*> *data);
	int improvesRatio(vector<BaseEntity*> *currentRow, double nextEntity, int length);

	EntityData *entityData;
	int width, height;
};
