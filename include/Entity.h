#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Entity
{
public:
	Entity (string csvLine);
	string prefix;
	string id;
	vector <string> data;
	float value	;
};

#endif
