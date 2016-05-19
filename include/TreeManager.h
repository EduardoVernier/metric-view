#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#include <iostream>
#include <vector>
#include <algorithm>
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
	TreeManager(){ make_heap (packageVector.begin(),packageVector.end(),compPackages()); };
	void addEntity(Entity ent);
	
	vector<Package> packageVector;

private:

};

#endif