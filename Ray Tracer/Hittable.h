#ifndef HITTABLE_H
#define HITTABLE_H

#include "AxisAlignedBoundingBox.h"

//Including Material.h here would cause circular include dependency problem, which can result in bad undefined behaviour
//depending upon the compiler used. This is because, this header (Hittable.h) is already included in Material.h.
//To avoid this, we will do forward declaration of Material class.
class Material;

class HitRecord
{
public:
	Point3 m_point;
	Vec3 m_normal;
	shared_ptr<Material> m_material;
	double m_t; //Ray parameter at hit point.
	double m_u; //u texture coordinate at hit point.
	double m_v; //v texture coordinate at hit point.
	bool m_frontFace;

	
	//Used to store whether ray hits the hittable from inside surface or outside surface.
	//Parameter outwardNormal is assumed to be a unit vector.
	//m_normal is set in such a way that it always points against the incoming ray.
	void m_setFaceNormal(const Ray& r, const Vec3& outwardNormal);
};

//Abstract class
class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const = 0;
	virtual AxisAlignedBoundingBox m_getBoundingBox() const = 0;
};

#endif