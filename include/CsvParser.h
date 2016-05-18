#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <iostream>
#include <dirent.h>

using namespace std;

class CsvParser
{
public:
	CsvParser(string dataDir);
	
private:
	string lastRev;
};


#endif
