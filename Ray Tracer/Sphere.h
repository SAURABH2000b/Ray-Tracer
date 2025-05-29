#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius, shared_ptr<Material> material) : 
		m_center(center), 
		m_radius(std::fmax(radius, 0)),
		m_material(material) {}
	bool  m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const override;
private:
	Point3 m_center;
	double m_radius;
	shared_ptr<Material> m_material;
};

#endif