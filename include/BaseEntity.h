#ifndef BASEENTITY_H
#define BASEENTITY_H
#include <string>

// Abstract class
class BaseEntity
{
public:
	virtual float getScore() = 0;
	virtual std::string getName() = 0;
	void setLevel(int l) { level = l; };
	int getLevel() { return level; }
	void setAsPackage() { packageFlag = 1; };
	int isPackage() { return packageFlag; };
	void setNormalizedScore(float s) { normalizedScore = s; };
	float getNormalizedScore() { return normalizedScore; };
	int coords [4] = {-1,-1,-1,-1};

protected:
	float normalizedScore = -1;
	int packageFlag = 0;
	int level = -1;
};

#endif
