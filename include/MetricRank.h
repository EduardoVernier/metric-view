#ifndef METRICRANK_H
#define METRICRANK_H

#include <vector>
#include <algorithm>
#include <cmath>

#include "Entity.h"
#include "EntityTree.h"
#include "Colormap.h"

using namespace std;

class MetricRank
{
public:
	MetricRank(EntityTree *_et);
	void computeLocalGroupMetric(unsigned Rt);

	vector<vector<unsigned char>> explainingMetric;

private:
	void computeMean(vector<Entity*> entityVector, unsigned Rt, vector<double> &meanVector);
	void computeStd(vector<Entity*> entityVector, unsigned Rt, vector<double> &stdVector);

	EntityTree *et;
};

#endif
