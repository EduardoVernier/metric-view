#ifndef TREEMAP_H
#define TREEMAP_H

#include <iostream>
#include <vector>
#include <cfloat>
#include "math.h"

#include "BaseEntity.h"
#include "EntityTree.h"
#include "Container.h"

using namespace std;

class Treemap
{
public:
	Treemap(EntityTree et, int _width, int _height);
	void treemapMultidimensional(vector<BaseEntity*> *data, float width, float height, float xOffset, float yOffset);
	void treemapSingledimensional(vector<BaseEntity*> *data, float width, float height, float xOffset, float yOffset);
	void squarify(vector<BaseEntity*> *data, vector<BaseEntity*> *currentRow, Container container);
	void normalize(vector<BaseEntity*> *data, int area);
	int sumScores(vector<BaseEntity*> *data);
	float sumNormalizedScores(vector<BaseEntity*> *data);
	int improvesRatio(vector<BaseEntity*> *currentRow, float nextEntity, int length);

	EntityTree entityTree;
	int width, height;
};

#endif
