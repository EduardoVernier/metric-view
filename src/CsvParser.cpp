#include "../include/CsvParser.h"

CsvParser::CsvParser(string dataDirName)
{
	
	int len;
	struct dirent *dDirent;
	DIR *dataDir;
	
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
	cout << lastRev <<  endl;
	closedir (dataDir);	
}
