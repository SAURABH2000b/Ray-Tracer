#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	//Stationary Sphere
	Sphere(const Point3& staticCenter, double radius, shared_ptr<Material> material);

	//Moving Sphere
	Sphere(const Point3& center1, const Point3& center2, double radius, shared_ptr<Material> material);

	bool m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const override;
	AxisAlignedBoundingBox m_getBoundingBox() const override;
private:
	static void s_getSphereUV(const Point3& p, double& u, double& v);

	Ray m_center;
	double m_radius;
	shared_ptr<Material> m_material;
	AxisAlignedBoundingBox m_boundingBox;
};

#endif