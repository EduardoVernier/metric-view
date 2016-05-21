#ifndef TREEMANAGER_H
#define TREEMANAGER_H

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

class TreeManager
{
public:
	TreeManager(){ };
	void addEntity(Entity ent);
	void buildHierarchy();
	void sortPackages(Package *p);
  void setHierarchicalLevel(Package *p, int level);
	void generateOutputVector(Package *p);

	vector<Package> packageVector;
	vector<BaseEntity*> output;
};

#endif
