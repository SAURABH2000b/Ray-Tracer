#ifndef AXIS_ALIGNED_BOUNDING_BOX_H
#define AXIS_ALIGNED_BOUNDING_BOX_H

#include "Utility.h"

class AxisAlignedBoundingBox
{
public:
	Interval m_x, m_y, m_z;

	//The default AxisAlignedBoundingBox is empty, since intervals are empty by default.
	AxisAlignedBoundingBox() {}

	AxisAlignedBoundingBox(const Interval& x, const Interval& y, const Interval& z)
		: m_x(x), m_y(y), m_z(z) {}
	AxisAlignedBoundingBox(const Point3& a, const Point3& b);
	AxisAlignedBoundingBox(const AxisAlignedBoundingBox& box0, const AxisAlignedBoundingBox& box1);

	const Interval& m_getAxisInterval(int n) const;
	bool m_hit(const Ray& r, Interval tRay) const;

	int m_getLongestAxis() const;

	static const AxisAlignedBoundingBox s_empty, s_universe;
};

#endif