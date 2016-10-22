#include "../include/Container.h"

// Initialize Container
Container::Container(double _width, double _height, double _xOffset, double _yOffset) {

    width = _width;
    height = _height;
    xOffset = _xOffset;
    yOffset = _yOffset;

    shortestEdge = (width < height) ? width : height;
}

// Subtract area from this container and return a new container
Container Container::cutArea(double area) {

    if (width >= height) {
        double areaWidth = area / height;
        double newWidth = width - areaWidth;
        return Container(newWidth, height, xOffset + areaWidth, yOffset);
    } else {
        double areaHeight = area / width;
        double newHeight = height - areaHeight;
        return Container(width, newHeight, xOffset, yOffset + areaHeight);
    }
}

// Save coordinates that correspond to each element on the treemap into BaseEntity coords member
void Container::saveCoordinates(vector<BaseEntity *> *row, double scoreSum) {

    double subxOffset = xOffset, subyOffset = yOffset; // Offset within the container
    double areaWidth = scoreSum / height;
    double areaHeight = scoreSum / width;
    double coordinates[4];

    if (width >= height) {
        for (unsigned i = 0; i < row->size(); i++) {
            coordinates[0] = subxOffset;
            coordinates[1] = subyOffset;
            coordinates[2] = subxOffset + areaWidth;
            coordinates[3] = subyOffset + (*row)[i]->getNormalizedScore() / areaWidth;
            (*row)[i]->setCoords(coordinates);
            subyOffset = subyOffset + (*row)[i]->getNormalizedScore() / areaWidth;
        }
    } else {
        for (unsigned i = 0; i < row->size(); i++) {
            coordinates[0] = subxOffset;
            coordinates[1] = subyOffset;
            coordinates[2] = subxOffset + (*row)[i]->getNormalizedScore() / areaHeight;
            coordinates[3] = subyOffset + areaHeight;
            (*row)[i]->setCoords(coordinates);
            subxOffset = subxOffset + (*row)[i]->getNormalizedScore() / areaHeight;
        }
    }
};
