#ifndef ENTITYTREE_H
#define ENTITYTREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <cmath>

#include "BaseEntity.h"
#include "Entity.h"
#include "Package.h"

using namespace std;

struct compPackages
{
	bool operator() (const Package& p1, const Package& p2) const
	{
		return (p1.sum > p2.sum);
	}
};

class EntityTree
{
public:
	EntityTree(){ };
	void addEntity(Entity ent);
 	void buildHierarchy();
	void generateSortedEntitiesVector(Package *p);
	void printTree();
	double getMin() { return treeMin; };
	double getMax() { return treeMax; };
	Entity* getEntityByName(string prefix, string id);
	void getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click, unsigned ctrlDown);
	void addProjection(string name, double x, double y, unsigned index);
	void normalizeProjection(int shortEdge);
	void setColorMetric(int mIndex);
	int getColorMetric() { return colorMetricIndex; };
	float getCMMin() { return colorMetricMin; };
	float getCMMax() { return colorMetricMax; };
	void setRadiusMetric(int mIndex);
	int getRadiusMetric() { return radiusMetricIndex; };
	float getRMMin() { return radiusMetricMin; };
	float getRMMax() { return radiusMetricMax; };

	vector<string> metricVector;
	vector<Entity*> selected;
	Entity* hovered;
	vector<Package> packageVector; // Entity tree is a collection of Packages
	vector<BaseEntity*> sortedEntities; // Contains both Packages and Entities ordered by score
	int depth = 0;
	unsigned nRevisions = 0;

private:
	void sortPackages(Package *p);
	void setHierarchicalLevel(Package *p, int level);
	void setFirstLevelId(Package *p, int level);
	void setMinMax();

	int radiusMetricIndex = 21;
	float radiusMetricMin;
	float radiusMetricMax;

	int colorMetricIndex = 21;
	float colorMetricMin;
	float colorMetricMax;

	double treeMin, treeMax; // Initial LOC score
	double minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;

	int firstLevelGlobalCounter = 0;
};

#endif
