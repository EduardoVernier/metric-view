#include "../include/BaseEntity.h"

void BaseEntity::setCoords(double *_coords) {

    for (int i = 0; i < 4; ++i) {
        coords[i] = _coords[i];
    }
}

double BaseEntity::getCoord(int i) {

    return coords[i];
}
