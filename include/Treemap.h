#ifndef TREEMAP_H
#define TREEMAP_H 

#include <vector>

#include "BaseEntity.h" 
#include "EntityTree.h"
#include "Container.h"

using namespace std;

class Treemap
{
public:
	Treemap(EntityTree et, int _width, int _height);
	void treemapMultidimensional(vector<BaseEntity*> *data, int width, int height, int xOffset, int yOffset);
	void treemapSingledimensional(vector<BaseEntity*> *data, int width, int height, int xOffset, int yOffset);
	void squarify(vector<BaseEntity*> *data, vector<BaseEntity*> *currentRow, Container container, vector<BaseEntity*> *stack);
	void normalize(vector<BaseEntity*> *data, int area);
	int sumScores(vector<BaseEntity*> *data);


	EntityTree entityTree;
	int width, height;
};

#endif