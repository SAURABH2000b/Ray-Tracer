#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>

class HittableList : public Hittable
{
public:
	std::vector<shared_ptr<Hittable>> m_objects;

	HittableList() {}
	HittableList(shared_ptr<Hittable> object);

	void m_clear();
	void m_add(shared_ptr<Hittable> object);

	bool m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const override;

	AxisAlignedBoundingBox m_getBoundingBox() const override;

private:
	AxisAlignedBoundingBox m_boundingBox;
};

#endif