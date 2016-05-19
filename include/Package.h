#ifndef PACKAGE_H
#define PACKAGE_H

#include <algorithm>
#include <vector>
#include "Entity.h"
using namespace std;

struct compEntities 
{
    bool operator() (const Entity& e1, const Entity& e2) const
    {
    	return (e1.value > e2.value);
    }
};

class Package
{
public:
	Package(string _prefix) : prefix(_prefix){sum=0;};
	void addEntity(Entity ent);
	
	vector<Entity> entityVector;
	string prefix;
	float sum;
};

#endif
