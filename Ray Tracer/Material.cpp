#include "Material.h"

//Diffuse: Uniform random distribution of scattered rays
//Here reflected ray is selected randomly from uniform distribution on the unit hemisphere at the point of intersetion, on the side of normal.
bool UniformMatte::m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const {

	Vec3 scatterDirection = g_randomOnHemisphere(record.m_normal);
	scatteredRay = Ray(record.m_point, scatterDirection);
	attenuation = m_albedo;
	return true;

}

//Diffuse: Lambartian model
//Here scattered ray is selected randomly from uniform distribution on the unit sphere, tangent to point of intersetion, in the direction of normal.
bool Lambertian::m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const {

	//input ray is not scattered
	if (g_randomDouble() > m_scatteringProbability)
		return false;

	Vec3 scatterDirection = record.m_normal + g_randomUnitVector();
	//Check if scatterDirection is close to zero, if yes use normal as the scatterDirection.
	if (scatterDirection.m_nearZero())
		scatterDirection = record.m_normal;

	scatteredRay = Ray(record.m_point, scatterDirection);
	attenuation = m_albedo / m_scatteringProbability;
	return true;

}

bool Metal::m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const {

	Vec3 reflectedDirection = g_reflect(inputRay.m_getDirection(), record.m_normal);

	//There is a sphere with radius fuzz at the end point of the normalized reflectedDirection vector.
	//We select a random point on this sphere. The vector from point of hit to this random point then becomes new reflectedDirection.
	reflectedDirection = g_unitVector(reflectedDirection) + (m_fuzz * g_randomUnitVector());
	if (g_dot(reflectedDirection, record.m_normal) < 0)
		return false;
	scatteredRay = Ray(record.m_point, reflectedDirection);
	attenuation = m_albedo;
	return true;

}

bool Dielectric::m_scatter(const Ray& inputRay, const HitRecord& record, Color& attenuation, Ray& scatteredRay) const {
	//Input ray is reflected
	if (g_randomDouble() > m_refractionProbability) {
		Vec3 reflectedDirection = g_reflect(inputRay.m_getDirection(), record.m_normal);
		reflectedDirection = g_unitVector(reflectedDirection) + (m_fuzz * g_randomUnitVector());
		if (g_dot(reflectedDirection, record.m_normal) < 0)
			return false;
		scatteredRay = Ray(record.m_point, reflectedDirection);
		attenuation = m_albedo;
		return true;
	}
	//Input ray is refracted
	else {
		attenuation = Color(1.0, 1.0, 1.0);
		double refractiveIndex = record.m_frontFace ? (1.0 / m_refractionIndex) : m_refractionIndex;
		Vec3 unitDirection = g_unitVector(inputRay.m_getDirection());

		double cosTheta = std::fmin(g_dot(-unitDirection, record.m_normal), 1.0);
		double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

		//Total internal reflection
		bool cannotRefract = refractiveIndex * sinTheta > 1.0;
		Vec3 scatteredDirection;

		if (cannotRefract || g_reflectance(cosTheta, refractiveIndex) > g_randomDouble())
			scatteredDirection = g_reflect(unitDirection, record.m_normal);
		else
			scatteredDirection = g_refract(unitDirection, record.m_normal, refractiveIndex);

		scatteredRay = Ray(record.m_point, scatteredDirection);
		return true;
	} 
}

double Dielectric::g_reflectance(double cosine, double refractiveIndex)
{
	//Using Schlick's approzimation for reflectance.
	auto r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
	r0 = r0 * r0;
	return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}
