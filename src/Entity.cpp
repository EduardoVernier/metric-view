#include "../include/Entity.h"

Entity::Entity (string csvLine)
{
	// Collect all data
	stringstream ss(csvLine);
    string item;
    while (getline(ss, item, ';'))
    {
        data.push_back(item);
    }
    string name = data[0];
    size_t separator = name.rfind(".");
    id = name.substr(separator+1, name.length());
    prefix = name.substr(0,separator);
    
    // CountLine metric as value  
    istringstream buffer(data[22]);
    buffer >> value; 
}