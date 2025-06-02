#include "Utility.h"
#include "HittableList.h"

HittableList::HittableList(shared_ptr<Hittable> object)
{
	m_add(object);
}

void HittableList::m_clear()
{
	m_objects.clear();
}

void HittableList::m_add(shared_ptr<Hittable> object)
{
	m_objects.push_back(object);
	m_boundingBox = AxisAlignedBoundingBox(m_boundingBox, object->m_getBoundingBox());
}


bool HittableList::m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const
{
	HitRecord  tempRecord;
	bool hitAnything = false;
	auto closestSoFar = rayInterval.m_max;

	for (const auto& object : m_objects) {
		if (object->m_hit(r, Interval(rayInterval.m_min, closestSoFar), tempRecord)) {
			hitAnything = true;
			closestSoFar = tempRecord.m_t;
			record = tempRecord;
		}
	}

	return hitAnything;
}

AxisAlignedBoundingBox HittableList::m_getBoundingBox() const
{
	return m_boundingBox;
}
