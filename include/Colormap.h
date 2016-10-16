#pragma once

#include <cmath>
#include "EntityData.h"
#include "Color.h"
#include "Package.h"
#include "Entity.h"
extern EntityData *entityData;

enum class COLORMAP : int { sequential = 0, qualitative, divergent };

extern Color divergent[5];
extern Color sequential[5];

Color sequentialColormap(double v);
Color divergentColormap(double v);
Color qualitativeColormap(int v);
Color getColor(int colormap, BaseEntity *b, unsigned Rt);
