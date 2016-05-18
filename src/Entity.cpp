#include "../include/Entity.h"

Entity::Entity (string csvLine)
{
	stringstream ss(csvLine);
    string item;
    while (getline(ss, item, ';'))
    {
        data.push_back(item);
    }
    cout << data[0];

}
