#include <iostream>
#include <string.h>
#include <dirent.h>

#include "../include/CsvParser.h"
#include "../include/TreeManager.h"

using namespace std;

TreeManager *treeManager = new TreeManager();

int main (int argc, char** argv)
{
	// Check if a string was inputed
	if (argc < 2) 
	{
		cout << ("Usage: runner <dirname>\n");
		return 1;
	}
	
	CsvParser parser (argv[1]);
	
	return 0;
}
