#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <sstream>
#include <vector>

#include "Point.h"
#include "BaseEntity.h"

using namespace std;

class Entity : public BaseEntity
{
public:
	Entity (string csvLine, unsigned nRevisions);
	// Overriding baseclass virtual methods
	double getScore(){ return value; };
	string getName(){ return id; };
	string getPrefix() { return prefix; };
	void addPointAtIndex(Point p, unsigned index);

	vector <Point> projectionPoints;
	double value; // too much trouble to make private because of custom comparator

private:
	string prefix;
	string id;
	vector <string> data;
};

#endif
