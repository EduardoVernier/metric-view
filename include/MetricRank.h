#ifndef METRICRANK_H
#define METRICRANK_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <utility> // pair
#include <iostream>

#include "Entity.h"
#include "EntityTree.h"
#include "Colormap.h"

using namespace std;

class MetricRank
{
public:
	MetricRank(EntityTree *_et);
	void computeLocalGroupMetric(unsigned Rt);

	vector<pair<unsigned,double>> explainingMetric;

private:
	void computeMean(vector<Entity*> entityVector, unsigned Rt, vector<double> &meanVector);
	void computeVar(vector<Entity*> entityVector, unsigned Rt, vector<double> &varVector);

	EntityTree *et;
};

#endif
