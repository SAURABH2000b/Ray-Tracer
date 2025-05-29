#include "Utility.h"
#include "Hittable.h"

void HitRecord::m_setFaceNormal(const Ray& r, const Vec3& outwardNormal)
{
	m_frontFace = g_dot(r.m_getDirection(), outwardNormal) < 0;
	m_normal = m_frontFace ? outwardNormal : -outwardNormal;
}
