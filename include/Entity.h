#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include "Point.h"
#include "BaseEntity.h"

using namespace std;

class Entity : public BaseEntity
{
public:
	Entity (string csvLine, unsigned nAttributes, unsigned nRevisions);
	// Overriding baseclass virtual methods
	double getScore();
	string getName(){ return id; }
	string getPrefix() { return prefix; }
	void addPointAtIndex(Point p, unsigned index);
	unsigned isSelected() { return selected; }
	void setSelected(unsigned s) { selected = s; }
	void addRevisionData (string data, unsigned rev, int nAttributes);

	vector <vector <double>> data; // data[revision][metric]
	vector <vector <double>> normalizedData;
	vector <Point> projectionPoints;
	vector <Point> normalizedProjectionPoints;

	double value; // too much trouble to make private because of custom comparator
	bool showHalo = false;

    Point getPosition(unsigned int Rt, double animationStep);

private:
	string prefix = "";
	string id;
	unsigned selected = 0;
};
