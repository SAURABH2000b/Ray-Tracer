#include "AxisAlignedBoundingBox.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Point3& a, const Point3& b)
{
	//Treat the two points a and b as extrema for the bounding box, so we don't require a
	//particular minimum/maximum coordinate order.

	m_x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
	m_y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
	m_z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const AxisAlignedBoundingBox& box0, const AxisAlignedBoundingBox& box1)
{
	m_x = Interval(box0.m_x, box1.m_x);
	m_y = Interval(box0.m_y, box1.m_y);
	m_z = Interval(box0.m_z, box1.m_z);
}

const Interval& AxisAlignedBoundingBox::m_getAxisInterval(int n) const
{
	if (n == 1) return m_y;
	if (n == 2) return m_z;
	return m_x;
}

bool AxisAlignedBoundingBox::m_hit(const Ray& r, Interval tRay) const
{
	const Point3& rayOrigin = r.m_getOrigin();
	const Vec3& rayDirection = r.m_getDirection();

	for (int axis = 0; axis < 3; axis++) {
		const Interval& axisInterval = m_getAxisInterval(axis);
		const double adinv = 1.0 / rayDirection[axis];

		auto t0 = (axisInterval.m_min - rayOrigin[axis]) * adinv;
		auto t1 = (axisInterval.m_max - rayOrigin[axis]) * adinv;

		//We will shrink the tRay interval based upon the t limits calculated for the current axis.
		if (t0 < t1) {
			if (t0 > tRay.m_min) tRay.m_min = t0;
			if (t1 < tRay.m_max) tRay.m_max = t1;
		}
		else {
			if (t1 > tRay.m_min) tRay.m_min = t1;
			if (t0 < tRay.m_max) tRay.m_max = t0;
		}

		//If the current t limits makes the tRay inter as 0 width or invalid (tRay.m_min > tRay.m_max)
		//then it means the ray is not contained in the bounding box for any value of t.
		if (tRay.m_max <= tRay.m_min)
			return false;
	}
	return true;
}

int AxisAlignedBoundingBox::m_getLongestAxis() const
{
	//Returns the index of the longest axis of the bounding box.
	if (m_x.m_size() > m_y.m_size())
		return m_x.m_size() > m_z.m_size() ? 0 : 2;
	else
		return m_y.m_size() > m_z.m_size() ? 1 : 2;
}

const AxisAlignedBoundingBox AxisAlignedBoundingBox::s_empty = AxisAlignedBoundingBox(Interval::s_empty, Interval::s_empty, Interval::s_empty);
const AxisAlignedBoundingBox AxisAlignedBoundingBox::s_universe = AxisAlignedBoundingBox(Interval::s_universe, Interval::s_universe, Interval::s_universe);
