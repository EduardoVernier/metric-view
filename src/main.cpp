#include <iostream>
#include <string.h>
#include <dirent.h>

#include "../include/CsvParser.h"

using namespace std;

int main (int argc, char** argv)
{
	// Check if a string was inputed
	if (argc < 2) 
	{
		cout << ("Usage: testprog <dirname>\n");
		return 1;
	}
	
	CsvParser parser (argv[1]);
	
	return 0;
}
