#include "../include/BaseEntity.h"

void BaseEntity::setCoords(float *_coords)
{
	for (int i = 0; i < 4; ++i)
	{
		coords[i] = _coords[i];
	}
}