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

class EntityTree
{
public:
	EntityTree(){ };
	void addEntity(Entity ent);
 	void buildHierarchy();
	void generateSortedEntitiesVector(Package *p);
	void generateEntityVector();
	void printTree();
	double getMin() { return treeMin; };
	double getMax() { return treeMax; };
	Entity* getEntityByName(string prefix, string id);
	void addProjection(string name, double x, double y, unsigned index);
	void normalizeData();
	void normalizeProjection(double shortEdge);
	void rankFastestChangingEntities(unsigned Rt, int direction);

	vector<string> metricVector;
	vector<Entity*> selected;
	BaseEntity *hovered = NULL;
	vector<Package> packageVector; // Entity tree is a collection of Packages
	vector<BaseEntity*> sortedBaseEntities; // Contains both Packages and Entities ordered by score
	vector<Entity*> entities; // Contains only Entities ordered by score
	int depth = 0;
	unsigned nRevisions = 0;
	unsigned nDimentions = 0;

private:
	void sortPackages(Package *p);
	void setHierarchicalLevel(Package *p, int level);
	void setFirstLevelId(Package *p, int level);
	void setMinMax();

	int radiusMetricIndex = 21;
	double radiusMetricMin;
	double radiusMetricMax;

	int colorMetricIndex = 21;
	double colorMetricMin;
	double colorMetricMax;

	int streamMetricIndex = 21;
	double streamMetricMin;
	double streamMetricMax;

	double treeMin, treeMax; // Initial LOC score
	double minX = DBL_MAX, maxX = DBL_MIN, minY = DBL_MAX, maxY = DBL_MIN;

	int firstLevelGlobalCounter = 0;

};

#endif
