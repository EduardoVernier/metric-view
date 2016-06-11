#include "../include/Colormap.h"

Color divergent[5] =
{Color(0.792,0,0.125),
 Color(0.956,0.647,0.509),
 Color(0.968,0.968,0.968),
 Color(0.572,0.733,0.870),
 Color(0.019,0.443,0.690)};

/* Beige to red colormap
 */
Color sequential[5] = {
	Color(0.996,0.941,0.850),
	Color(0.992,0.8,0.541),
	Color(0.988,0.552,0.349),
	Color(0.890,0.290,0.2),
	Color(0.701,0,0)};

/* Green to blue
Color sequential[5] =
	{Color(1,1,0.8),
		Color(0.631,0.864,0.705),
		Color(0.254,0.713,0.768),
		Color(0.172,0.498,0.721),
		Color(0.145,0.203,0.580)};
*/

/*
Yellow to Red
Color sequential[5] = {Color(1,1,0.698),
		Color(0.996078, 0.8, 0.360784),
		Color(0.992157, 0.552941, 0.235294),
		Color(0.941176, 0.231373, 0.12549),
		Color(0.741176, 0, 0.14902)};
*/
Color sequentialColormap(double v)
{
	double d = v;
	while (d > 0.25) d-=0.25;
	d*=4;

	int f = int(floor(v*4));
	int c = int(ceil(v*4));
	return (sequential[f]*(1-d)) + (sequential[c]*(d));
}
