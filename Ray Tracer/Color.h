#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"

using Color = Vec3;

//Color utility functions:
void g_writeColor(std::ostream& out, const Color& pixelColor);
extern inline double g_linearToGamma(double linearComponent);
extern inline void g_linearToGamma(Color& linearColor);

#endif