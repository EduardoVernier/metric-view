#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <string>

// Abstract class
class BaseEntity
{
public:
	virtual double getScore() = 0;
	virtual std::string getName() = 0;
	void setLevel(int l) { level = l; };
	int getLevel() { return level; }
	void setAsPackage() { packageFlag = 1; };
	int isPackage() { return packageFlag; };
	void setNormalizedScore(double s) { normalizedScore = s; };
	double getNormalizedScore() { return normalizedScore; };
	void setCoords(double *_coords);
	double coords[4];

private:
	double normalizedScore = -1;
	int packageFlag = 0;
	int level = -1;
};

#endif
