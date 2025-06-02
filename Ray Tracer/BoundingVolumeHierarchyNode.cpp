#include "BoundingVolumeHierarchyNode.h"
#include <algorithm>

BoundingVolumeHierarchyNode::BoundingVolumeHierarchyNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end)
{
	m_boundingBox = AxisAlignedBoundingBox::s_empty;
	for (size_t objectIndex = start; objectIndex < end; objectIndex++)
		m_boundingBox = AxisAlignedBoundingBox(m_boundingBox, objects[objectIndex]->m_getBoundingBox());

	int axis = m_boundingBox.m_getLongestAxis();

	auto comparator = (axis == 0) ? s_boxXCompare : (axis == 1) ? s_boxYCompare : s_boxZCompare;

	size_t objectSpan = end - start;

	if (objectSpan == 1) {
		m_left = m_right = objects[start];
	}
	else if (objectSpan == 2) {
		m_left = objects[start];
		m_right = objects[start + 1];
	}
	else {
		std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

		auto mid = start + objectSpan / 2;
		m_left = make_shared<BoundingVolumeHierarchyNode>(objects, start, mid);
		m_right = make_shared<BoundingVolumeHierarchyNode>(objects, mid, end);
	}
}

bool BoundingVolumeHierarchyNode::m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const
{
	if (!m_boundingBox.m_hit(r, rayInterval))
		return false;

	//First check if left partition is getting hit by the ray.
	//If yes, then the record parameter will be updated by the record of closest hit.
	bool hitLeft = m_left->m_hit(r, rayInterval, record);

	//Now while checking whether the ray hits right partition, we can pass an updated interval to the m_hit() function.
	//The minimum will be same as earlier, but if the ray hits left partition, then maximum (for right) will be the value of t
	//fetched from the left partition hit record.
	bool hitRight = m_right->m_hit(r, Interval(rayInterval.m_min, hitLeft ? record.m_t : rayInterval.m_max), record);

	return hitLeft || hitRight;
}

AxisAlignedBoundingBox BoundingVolumeHierarchyNode::m_getBoundingBox() const
{
	return m_boundingBox;
}

bool BoundingVolumeHierarchyNode::s_boxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisIndex)
{
	auto aAxisInterval = a->m_getBoundingBox().m_getAxisInterval(axisIndex);
	auto bAxisInterval = b->m_getBoundingBox().m_getAxisInterval(axisIndex);
	return aAxisInterval.m_min < bAxisInterval.m_min;
}

bool BoundingVolumeHierarchyNode::s_boxXCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return s_boxCompare(a, b, 0);
}

bool BoundingVolumeHierarchyNode::s_boxYCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return s_boxCompare(a, b, 1);
}

bool BoundingVolumeHierarchyNode::s_boxZCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return s_boxCompare(a, b, 2);
}
