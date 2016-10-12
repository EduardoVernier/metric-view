#include "../include/MetricRank.h"

MetricRank::MetricRank(EntityData *ed)
{
	entityData = ed;
	explainingMetric.resize(entityData->metricVector.size());
}

void MetricRank::computeLocalGroupMetric(unsigned Rt)
{
	unsigned nDimensions = entityData->nDimensions;
	vector<double> globalVar(nDimensions, 0);
	vector<double> localVar(nDimensions, 0);

	computeVar(entityData->entities, Rt, globalVar);
	computeVar(entityData->selected, Rt, localVar);

	double norm = 0;
	for (unsigned metric = 0; metric < nDimensions; ++metric)
		norm += localVar[metric]/globalVar[metric];

	for (unsigned metric = 0; metric < nDimensions; ++metric)
	{
		explainingMetric[metric].index = metric;
		explainingMetric[metric].score = (localVar[metric]/globalVar[metric])/norm;
	}

	sort(explainingMetric.begin(),explainingMetric.end(),
				[](const MetricScore &lhs, const MetricScore &rhs)
					{ return lhs.score < rhs.score; });

	// Print results
//	cout << string( 100, '\n' );
//	for_each(explainingMetric.begin(),explainingMetric.end(),[=](const MetricScore &a)
//	{
//		cout << std::setw(28) << entityData->metricVector[a.index] << "\tScore: " << setw(10) << a.score;
//		cout << "\tMean: "<< setw(10) << a.mean << "\tVariance: " << a.variance << endl;
//	});
}

void MetricRank::computeMean(vector<Entity*> entityVector, unsigned Rt, vector<double> &meanVector)
{
	for (unsigned metric = 0; metric < entityData->nDimensions; ++metric)
	{
		for (auto entity : entityVector)
		{
			meanVector[metric] += entity->data[Rt][metric];
		}
		meanVector[metric] /= double(entityVector.size());
		explainingMetric[metric].mean = meanVector[metric];
	}
}

void MetricRank::computeVar(vector<Entity*> entityVector, unsigned Rt, vector<double> &varVector)
{
	unsigned nDimensions = entityData->nDimensions;

	vector<double> meanVector (nDimensions, 0);
	computeMean(entityVector, Rt, meanVector);

	for (unsigned metric = 0; metric < nDimensions; ++metric)
	{
		for (auto e : entityVector)
		{
			varVector[metric] += pow(e->data[Rt][metric] - meanVector[metric], 2);
		}
		varVector[metric] /= (double)(entityVector.size());
		explainingMetric[metric].variance = varVector[metric];
	}
}

