#include "Utility.h"
#include "Sphere.h"

Sphere::Sphere(const Point3& staticCenter, double radius, shared_ptr<Material> material) :
	m_center(staticCenter, Vec3(0.0, 0.0, 0.0)),
	m_radius(std::fmax(radius, 0)),
	m_material(material) 
{
	auto cornerVector = Vec3(m_radius, m_radius, m_radius);
	m_boundingBox = AxisAlignedBoundingBox(staticCenter - cornerVector, staticCenter + cornerVector);
}

Sphere::Sphere(const Point3& center1, const Point3& center2, double radius, shared_ptr<Material> material) :
	m_center(center1, center2 - center1),
	m_radius(std::fmax(radius, 0)),
	m_material(material) 
{
	auto cornerVector = Vec3(m_radius, m_radius, m_radius);
	AxisAlignedBoundingBox box1(m_center.m_at(0) - cornerVector, m_center.m_at(0) + cornerVector); //for time = 0 or t=0 for m_center
	AxisAlignedBoundingBox box2(m_center.m_at(1) - cornerVector, m_center.m_at(1) + cornerVector); //for time = 1 or t=1 for m_center
	m_boundingBox = AxisAlignedBoundingBox(box1, box2);
}

bool Sphere::m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const
{
	Point3 currentCenter = m_center.m_at(r.m_getTime());
	Vec3 oc = currentCenter - r.m_getOrigin();
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
	Vec3 outwardNormal = (record.m_point - currentCenter) / m_radius;
	record.m_setFaceNormal(r, outwardNormal);
	s_getSphereUV(outwardNormal, record.m_u, record.m_v); //here outwardNormal is considered as a point on the unit sphere centered at origin.
	record.m_material = m_material;

	return true;
}

AxisAlignedBoundingBox Sphere::m_getBoundingBox() const
{
	return m_boundingBox;
}

void Sphere::s_getSphereUV(const Point3& p, double& u, double& v)
{
	//p: a given point on the sphere of radius one, centered at the origin.
	//u: returned value [0, 1] of angle around the Y axis from X=-1.
	//v: returned value [0, 1] of angle from Y=-1 to Y=+1.
	//   <1 0 0> yields <0.50 0.50>   <-1 0 0> yields <0.00 0.50>
	//   <0 1 0> yields <0.50 1.00>   <0 -1 0> yields <0.50 0.00>
	//   <0 0 1> yields <0.25 0.50>   <0 0 -1> yields <0.75 0.50>

	auto theta = std::acos(-p.m_y());
	auto phi = std::atan2(-p.m_z(), p.m_x()) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
}
