#ifndef BASEENTITY_H
#define BASEENTITY_H

using namespace std;

// Abstract class
class BaseEntity
{
public:
	virtual float getScore() = 0;
	virtual string getName() = 0;
	void setLevel(int l) { level = l; };
	int getLevel() { return level; }
	void setAsPackage() { packageFlag = 1; };
	int isPackage() { return packageFlag; };

protected:
	int packageFlag = 0;
	int level = -1;
};

#endif
