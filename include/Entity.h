#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <sstream>
#include <vector>

#include "BaseEntity.h"

using namespace std;

class Entity : public BaseEntity
{
public:
	Entity (string csvLine);
	// Overriding baseclass virtual methods
	double getScore(){ return value; };
	string getName(){ return id; };

	string prefix;
	string id;
	vector <string> data;
	double value;
};

#endif
