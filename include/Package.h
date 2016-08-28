#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

#include "BaseEntity.h"
#include "Entity.h"

using namespace std;

class Package : public BaseEntity
{
public:
	Package(string _prefix) : prefix(_prefix){ setAsPackage(); };
	void addEntity(Entity ent);
	void addChild(Package child);
	void printPackage(int level);
	void sortEntities();

	// Overriding baseclass virtual methods
	double getScore(){ return sum; };
	string getName(){ return prefix; };

	vector<BaseEntity*> sortedEntities;
	vector<Package> childrenVector;
	vector<Entity> entityVector;
	string prefix;
	double sum = 0;
};
