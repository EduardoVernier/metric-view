#ifndef PACKAGE_H
#define PACKAGE_H

#include <vector>
#include "Entity.h"
using namespace std;

class Package
{
public:
	Package(string _prefix) : prefix(_prefix){};
	void addEntity(Entity ent);
	
	vector<Entity> entityVector;
	string prefix;
	float sum;
};

#endif
