#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <sstream>
#include <vector>

#include "BaseEntity.h"

using namespace std;

class Entity : BaseEntity
{
public:
	Entity (string csvLine);
	float getScore(){ return value; };
	string getName(){ return id; };


	string prefix;
	string id;
	vector <string> data;
	float value	;
};

#endif
