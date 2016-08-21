#include "../include/MetricRank.h"

MetricRank::MetricRank(EntityTree *_et)
{
	et = _et;
	explainingMetric.resize(et->metricVector.size());
}


void MetricRank::computeLocalGroupMetric(unsigned Rt)
{
	unsigned nDim = et->metricVector.size();
	vector<double> globalVar(nDim, 0);
	vector<double> localVar(nDim, 0);

	computeVar(et->entities, Rt, globalVar);
	computeVar(et->selected, Rt, localVar);

	double norm = 0;
	for (unsigned m = 0; m < nDim; ++m)
		norm += localVar[m]/globalVar[m];

	for (unsigned m = 0; m < nDim; ++m)
	{
		explainingMetric[m] = make_pair(m, (localVar[m]/globalVar[m])/norm);
	}
	sort(explainingMetric.begin(),explainingMetric.end(),
				[](const pair<unsigned, double>& lhs, const pair<unsigned, double>& rhs)
					{ return lhs.second < rhs.second; });

	cout << endl;
	for_each(explainingMetric.begin(),explainingMetric.end(),[=](const pair<unsigned, double>& a)
	{	cout << et->metricVector[a.first] << " " << a.second << endl;	});
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

void MetricRank::computeVar(vector<Entity*> entityVector, unsigned Rt, vector<double> &varVector)
{
	unsigned nDim = et->metricVector.size();

	vector<double> meanVector (nDim, 0);
	computeMean(entityVector, Rt, meanVector);

	for (unsigned m = 0; m < nDim; ++m)
	{
		for (vector<Entity*>::iterator b = entityVector.begin(); b != entityVector.end(); ++b)
		{
			varVector[m] += pow((*b)->data[Rt][m] - meanVector[m], 2);
		}
		varVector[m] /= (double)(entityVector.size());
	}
}

