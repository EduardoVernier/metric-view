#pragma once

#include <cmath>
#include "Color.h"

extern Color divergent[5];
extern Color sequential[5];

Color sequentialColormap(double v);
Color divergentColormap(double v);
Color qualitativeColormap(int v);
