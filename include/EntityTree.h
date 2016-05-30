#ifndef ENTITYTREE_H
#define ENTITYTREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cfloat>

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

	vector<Package> packageVector; // Entity tree is a collection of Packages
	vector<BaseEntity*> sortedEntities; // Contains both Packages and Entities ordered by score
	int depth = 0;

private:
	void sortPackages(Package *p);
	void setHierarchicalLevel(Package *p, int level);
	void setMinMax();
	double treeMin, treeMax;

};

#endif
