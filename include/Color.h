#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	float R, G, B;
	Color(float r, float g, float b) : R(r), G(g), B(b) {};
	Color operator+(const Color c)	{  return Color (R+c.R, G+c.G, B+c.B); };
	Color operator*(const double& b)	{	return Color (R*b, G*b, B*b); };
};

#endif
