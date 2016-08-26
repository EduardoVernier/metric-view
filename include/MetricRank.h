#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <utility> // pair
#include <iostream>
#include <iomanip>

#include "Entity.h"
#include "EntityTree.h"
#include "Colormap.h"

using namespace std;

struct MetricScore
{
	unsigned index;
	double score;
	double mean;
	double variance;
};

class MetricRank
{
public:
	MetricRank(EntityTree *_et);
	void computeLocalGroupMetric(unsigned Rt);

	vector<MetricScore> explainingMetric;

private:
	void computeMean(vector<Entity*> entityVector, unsigned Rt, vector<double> &meanVector);
	void computeVar(vector<Entity*> entityVector, unsigned Rt, vector<double> &varVector);

	EntityTree *et;
};
