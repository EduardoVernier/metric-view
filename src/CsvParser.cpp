#include "../include/CsvParser.h"

CsvParser::CsvParser(string dataDirName)
{
	int len;
	struct dirent *dDirent;
	DIR *dataDir;
	string lastRev;
	
	// Check if it is a valid directory
	dataDir = opendir(dataDirName.c_str());
	if (dataDir == NULL) 
	{
		cout << ("Cannot open directory '%s'\n", dataDirName);
		return;
	}
	
	// Find last revision file
	lastRev = "";
	while ((dDirent = readdir(dataDir)) != NULL)
	{
		string filename (dDirent->d_name);
		if (filename.length() >= lastRev.length())
		{
			lastRev = filename;
			if (dDirent->d_name > lastRev)
			{
				lastRev = filename;
			}
		}
	}
	closedir (dataDir);
	cout << dataDirName + '/' + lastRev <<  endl;
	parseMetricFile(dataDirName + '/' + lastRev);
}

void CsvParser::parseMetricFile(string filename)
{
	int nEntities, nAttributes;
	ifstream file(filename.c_str());
	string line;
	getline(file,line); // 'DY' - flush
	getline(file,line); // number of classes in file
	istringstream(line) >> nEntities;
	getline(file,line); // number of attributes
	istringstream(line) >> nAttributes;
	getline(file,line); // name of attributes - flush
	
	while(getline(file,line))
    {
        Entity *newEntity = new Entity(line);
    }
	
	
	


} 
