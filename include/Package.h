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

class Package : public BaseEntity
{
public:
	Package(string _prefix) : prefix(_prefix){ setAsPackage(); };
	void addEntity(Entity ent);
	void addChild(Package child);
	void printPackage(int level);
	void sortEntities();	

	// Overriding baseclass virtual methods
	float getScore(){ return sum; };
	string getName(){ return prefix; };

	vector<BaseEntity*> sortedEntities;
	vector<Package> childrenVector;
	vector<Entity> entityVector;
	string prefix;
	float sum = 0;
};

#endif
