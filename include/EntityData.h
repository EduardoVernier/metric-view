#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>

#include "BaseEntity.h"
#include "Entity.h"
#include "Package.h"
#include "Controller.h"

extern Controller controller;

using namespace std;

class EntityData
{
public:
	EntityData(){};
	void addEntity(Entity ent);
 	void buildHierarchy();
	void generateSortedEntitiesVector(Package *p);
	void generateEntityVector();
	void printTree();
	Entity* getEntityByName(string prefix, string id);
	void addProjection(string name, double x, double y, unsigned index);
	void normalizeData();
	void normalizeProjection(double shortEdge);
	void rankFastestChangingEntities(unsigned Rt, int direction);
	void updateSelectedEntities();
	void computeMostSimilarPairs(unsigned Rt);

	vector<string> metricVector;
	vector<Entity*> selected;
	BaseEntity *hovered = NULL;
	vector<Package> packageVector; // Entity tree is a collection of Packages
	vector<BaseEntity*> sortedBaseEntities; // Contains both Packages and Entities ordered by score
	vector<Entity*> entities; // Contains only Entities ordered by score
	vector<double> maxMetricValue;
	vector<double> minMetricValue;
	vector<pair<double, pair<Entity*, Entity*>>> similarityRank;
	int depth = 0;
	unsigned nRevisions = 0;
	unsigned nDimensions = 0;
	int firstLevelGlobalCounter = 0;

private:
	void sortPackages(Package *p);
	void setHierarchicalLevel(Package *p, int level);
	void setFirstLevelId(Package *p, int level);
	double nDEuclidianDistance(vector<double> &a, vector<double> &b);

	double minX = DBL_MAX, maxX = DBL_MIN, minY = DBL_MAX, maxY = DBL_MIN; // Used in the projection normalizationF

};