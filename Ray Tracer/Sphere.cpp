#include "Utility.h"
#include "Sphere.h"

bool Sphere::m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const
{
	Vec3 oc = m_center - r.m_getOrigin();
	auto a = r.m_getDirection().m_lengthSquared();
	auto h = g_dot(r.m_getDirection(), oc); //b = -2h
	auto c = oc.m_lengthSquared() - m_radius * m_radius;

	auto discriminant = h * h - a * c;
	if (discriminant < 0) {
		return false;
	}
	auto sqrtDiscriminant = std::sqrt(discriminant);

	//Find the nearest root that lies in the acceptable range.
	auto root = (h - sqrtDiscriminant) / a;
	if (root <= rayInterval.m_min || rayInterval.m_max <= root) {
		//if nearest root is not in th range, try for furthest root
		root = (h + sqrtDiscriminant) / a;
		if (root <= rayInterval.m_min || rayInterval.m_max <= root) {
			return false;
		}
	}

	record.m_t = root;
	record.m_point = r.m_at(root);
	Vec3 outwardNormal = (record.m_point - m_center) / m_radius;
	record.m_setFaceNormal(r, outwardNormal);
	record.m_material = m_material;

	return true;
}
