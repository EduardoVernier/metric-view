#pragma once

#include <cmath>
#include "EntityTree.h"
#include "Color.h"
#include "Package.h"
#include "Entity.h"
extern EntityTree *entityTree;

enum class COLORMAP: int { sequential = 0, qualitative, divergent };

extern Color divergent[5];
extern Color sequential[5];

Color sequentialColormap(double v);
Color divergentColormap(double v);
Color qualitativeColormap(int v);
Color getColor(int colormap, BaseEntity *b, unsigned Rt);
