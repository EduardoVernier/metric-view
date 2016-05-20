#ifndef PACKAGE_H
#define PACKAGE_H

#include <iostream>
#include <algorithm>
#include <vector>

#include "BaseEntity.h"
#include "Entity.h"

using namespace std;

struct compEntities 
{
    bool operator() (const Entity& e1, const Entity& e2) const
    {
    	return (e1.value > e2.value);
    }
};

class Package : BaseEntity
{
public:
	Package(string _prefix) : prefix(_prefix){isPackage = 1; sum=0; };	
	void addEntity(Entity ent);
	void addChild(Package child);
	void printPackage(int level);
	float getScore(){ return sum; };
	string getName(){ return prefix; };


	vector<Package> childrenVector;
	vector<Entity> entityVector;
	string prefix;
	float sum;
};

#endif
