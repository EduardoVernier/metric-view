#include "../include/Package.h"

void Package::addEntity(Entity ent)
{
	sum+=ent.value;
	entityVector.push_back(ent);
	sort(entityVector.begin(), entityVector.end(), compEntities());
}