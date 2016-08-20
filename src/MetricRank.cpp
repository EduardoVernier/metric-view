#include "../include/MetricRank.h"

MetricRank::MetricRank(EntityTree *_et)
{
	et = _et;
}

void MetricRank::computeLocalGroupMetric(unsigned Rt)
{
	unsigned nDim = et->metricVector.size();
	vector<double> globalStd(nDim, 0);
	vector<double> localStd(nDim, 0);

	computeStd(et->entities, Rt, globalStd);
	computeStd(et->selected, Rt, localStd);
}

void MetricRank::computeMean(vector<Entity*> entityVector, unsigned Rt, vector<double> &meanVector)
{
	unsigned nDim = et->metricVector.size();
	for (unsigned m = 0; m < nDim; ++m)
	{
		for (vector<Entity*>::iterator b = entityVector.begin(); b != entityVector.end(); ++b)
		{
			meanVector[m] += (*b)->data[Rt][m];
		}
		meanVector[m] /= (double)(entityVector.size());
	}
}

void MetricRank::computeStd(vector<Entity*> entityVector, unsigned Rt, vector<double> &stdVector)
{
	unsigned nDim = et->metricVector.size();

	vector<double> meanVector (nDim, 0);
	computeMean(entityVector, Rt, meanVector);

	for (unsigned m = 0; m < nDim; ++m)
	{
		for (vector<Entity*>::iterator b = entityVector.begin(); b != entityVector.end(); ++b)
		{
			stdVector[m] += pow((*b)->data[Rt][m] - meanVector[m], 2);
		}
		stdVector[m] /= (double)(entityVector.size());
		stdVector[m] = sqrt(stdVector[m]);
	}
}

