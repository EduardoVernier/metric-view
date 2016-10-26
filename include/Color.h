#ifndef COLOR_H
#define COLOR_H

class Color {
public:
    double R, G, B;

    Color(double r, double g, double b) : R(r), G(g), B(b) {};
    Color operator+(const Color c) { return Color(R + c.R, G + c.G, B + c.B); };
    Color operator*(const double &b) { return Color(R * b, G * b, B * b); };
    bool operator==(const Color &c) { return (c.R == R && c.G == G && c.B == B); };
};

#endif
