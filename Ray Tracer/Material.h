#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utility.h"
#include "Hittable.h"
#include "Texture.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const {
		return false;
	}
};

class UniformMatte : public Material {
public:
	UniformMatte(const Color& albedo) : m_texture(make_shared<SolidColor>(albedo)) {}
	UniformMatte(const shared_ptr<Texture> texture) : m_texture(texture) {}
	bool m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const override;
private:
	//Albedo is the fraction of incident light intensity, that is reflected by the surface.
	//Albedo can be diifferent for each color component.
	shared_ptr<Texture> m_texture;
};

class Lambertian : public Material {
public:
	Lambertian(const Color& albedo, const double scatteringProbability) : m_texture(make_shared<SolidColor>(albedo)), m_scatteringProbability(scatteringProbability) {}
	Lambertian(const shared_ptr<Texture> texture, const double scatteringProbability) : m_texture(texture), m_scatteringProbability(scatteringProbability) {}
	bool m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const override;
private:
	shared_ptr<Texture> m_texture;
	double m_scatteringProbability;
};

class Metal : public Material {
public:
	Metal(const Color& albedo, double fuzz) : m_texture(make_shared<SolidColor>(albedo)), m_fuzz(fuzz < 1 ? fuzz : 1) {}
	Metal(const shared_ptr<Texture> texture, double fuzz) : m_texture(texture), m_fuzz(fuzz < 1 ? fuzz : 1) {}
	bool m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const override;
private:
	shared_ptr<Texture> m_texture;

	//fuzz is to create fuzzy reflections. Lesser the fuzz, sharper the reflections.
	double m_fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double refractionIndex, const Color& albedo, double fuzz, double refractionProbability) : m_refractionIndex(refractionIndex),
													   m_texture(make_shared<SolidColor>(albedo)),
													   m_fuzz(fuzz),
													   m_refractionProbability(refractionProbability) {}
	Dielectric(double refractionIndex, const shared_ptr<Texture> texture, double fuzz, double refractionProbability) : m_refractionIndex(refractionIndex),
													   m_texture(texture),
													   m_fuzz(fuzz),
													   m_refractionProbability(refractionProbability) {
	}
	bool m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const override;
private:
	//Refractive index in vaccum or air, or the ratio of the material's refractive index
	//over the refractive index of the enclosing media.
	double m_refractionIndex;
	shared_ptr<Texture> m_texture; //To be used in case of reflected rays
	double m_fuzz; //To be used in case of reflected rays

	//Keep this as high (near to 1.0) as possible to mimic pure dielectric.
	double m_refractionProbability;

	//Schlick approximation for reflectance
	static double g_reflectance(double cosine, double refractiveIndex);
};

#endif