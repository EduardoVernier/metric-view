#ifndef ENTITYTREE_H
#define ENTITYTREE_H

#include <iostream>
#include <vector>
#include <algorithm>

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
	void sortPackages(Package *p);
 	void setHierarchicalLevel(Package *p, int level);
	void generateSortedEntitiesVector(Package *p);
	void printTree();

	vector<Package> packageVector;
	vector<BaseEntity*> sortedEntities;
	int depth = 0;
};

#endif
