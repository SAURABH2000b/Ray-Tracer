#ifndef RAY_H
#define RAY_H
#include "Vec3.h"

class Ray
{
public:
	Ray() {};
	Ray(const Point3& origin, const Vec3& direction, double time):
		m_origin(origin),
		m_direction(direction),
		m_time(time) { }
	Ray(const Point3& origin, const Vec3& direction):
		Ray(origin, direction, 0) { }

	const Point3& m_getOrigin() const { return m_origin; }
	const Vec3& m_getDirection() const { return m_direction; }
	const double m_getTime() const { return m_time; }

	Point3 m_at(double t) const {
		return m_origin + t * m_direction;
	}

private:
	Point3 m_origin;
	Vec3 m_direction;
	double m_time;
};

#endif