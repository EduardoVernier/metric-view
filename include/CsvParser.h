#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>

#include "Entity.h"

using namespace std;

class CsvParser
{
public:
	CsvParser(string dataDir);
	
private:
	void parseMetricFile(string filename); 
};


#endif
