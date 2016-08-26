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
		explainingMetric[m].index = m;
		explainingMetric[m].score = (localVar[m]/globalVar[m])/norm;
	}

	sort(explainingMetric.begin(),explainingMetric.end(),
				[](const MetricScore &lhs, const MetricScore &rhs)
					{ return lhs.score < rhs.score; });

	// Print results
	cout << string( 100, '\n' );
	for_each(explainingMetric.begin(),explainingMetric.end(),[=](const MetricScore &a)
	{
		cout << std::setw(28) << et->metricVector[a.index] << "\tScore: " << setw(10) << a.score;
		cout << "\tMean: "<< setw(10) << a.mean << "\tVariance: " << a.variance << endl;
	});
}

void MetricRank::computeMean(vector<Entity*> entityVector, unsigned Rt, vector<double> &meanVector)
{
	unsigned nDim = et->metricVector.size();
	for (unsigned m = 0; m < nDim; ++m)
	{
		for (auto e : entityVector)
		{
			meanVector[m] += e->data[Rt][m];
		}
		meanVector[m] /= double(entityVector.size());
		explainingMetric[m].mean = meanVector[m];
	}
}

void MetricRank::computeVar(vector<Entity*> entityVector, unsigned Rt, vector<double> &varVector)
{
	unsigned nDim = et->metricVector.size();

	vector<double> meanVector (nDim, 0);
	computeMean(entityVector, Rt, meanVector);

	for (unsigned m = 0; m < nDim; ++m)
	{
		for (auto e : entityVector)
		{
			varVector[m] += pow(e->data[Rt][m] - meanVector[m], 2);
		}
		varVector[m] /= (double)(entityVector.size());
		explainingMetric[m].variance = varVector[m];
	}
}

