#ifndef BASEENTITY_H
#define BASEENTITY_H

using namespace std;

// Abstract class
class BaseEntity
{
public:
	int isPackage;
	virtual float getScore() = 0; 
	virtual string getName() = 0; 
	
	//virtual void printBaseEntity() = 0;
};

#endif