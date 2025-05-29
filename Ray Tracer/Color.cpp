#include "Utility.h"

void g_writeColor(std::ostream& out, const Color& pixelColor)
{
	auto r = pixelColor.m_x();
	auto g = pixelColor.m_y();
	auto b = pixelColor.m_z();
	
	//Apply a linear to gamma transform for gamma 2
	r = g_linearToGamma(r);
	g = g_linearToGamma(g);
	b = g_linearToGamma(b);

	//Translate the [0,1] component values to the byte range [0,255].
	static const Interval s_intensity(0.000, 0.999);
	int rByte = int(255.999 * s_intensity.m_clamp(r));
	int gByte = int(255.999 * s_intensity.m_clamp(g));
	int bByte = int(255.999 * s_intensity.m_clamp(b));

	//Write out the pixel components for ppm format.
	out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

//gamma-2 transform
inline double g_linearToGamma(double linearComponent)
{
	if (linearComponent > 0) {
		return std::sqrt(linearComponent);
	}
}

//gamma-2 transform
inline void g_linearToGamma(Color& linearColor)
{
	double x = linearColor.m_x();
	double y = linearColor.m_y();
	double z = linearColor.m_z();
	if (x > 0 && y > 0 && z > 0) {
		linearColor = Color(std::sqrt(x), std::sqrt(y), std::sqrt(z));
	}
}
