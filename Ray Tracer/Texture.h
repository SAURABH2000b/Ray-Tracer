#ifndef TEXTURE_H
#define TEXTURE_H

#include "Utility.h"
#include "NativeImage.h"

//Textures will be of following types:
//1. Solid color texture or constant color texture: color is constant despite of uv coordinates or point location in 3D space.
//2. Spatial texture: color is dependent on only the location of point in 3D space.
//3. UV texture or image texture: color is dependent only on uv coordinates of point in 3D space.

class Texture
{
public:
	virtual ~Texture() = default;
	virtual Color m_value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture
{
public:
	SolidColor(const Color& albedo) : m_albedo(albedo) {}
	SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}
	Color m_value(double u, double v, const Point3& p) const override;
private:
	Color m_albedo;
};

class CheckerTexture : public Texture 
{
public:
	CheckerTexture(double xScale, double yScale, double zScale, shared_ptr<Texture> evenTexture, shared_ptr<Texture> oddTexture)
		: m_xInverseScale(1.0 / xScale), m_yInverseScale(1.0 / yScale), m_zInverseScale(1.0 / zScale), m_evenTexture(evenTexture), m_oddTexture(oddTexture) {}
	CheckerTexture(double xScale, double yScale, double zScale, const Color& c1, const Color& c2)
		: CheckerTexture(xScale, yScale, zScale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	Color m_value(double u, double v, const Point3& p) const override;
private:
	double m_xInverseScale;
	double m_yInverseScale;
	double m_zInverseScale;
	shared_ptr<Texture> m_evenTexture;
	shared_ptr<Texture> m_oddTexture;
};

class ImageTexture : public Texture
{
public:
	ImageTexture(const char* filename):m_image(filename) {}

	Color m_value(double u, double v, const Point3& p) const override;

private:
	NativeImage m_image;
};

#endif