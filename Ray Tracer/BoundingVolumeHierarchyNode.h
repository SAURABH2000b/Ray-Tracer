#ifndef BOUNDING_VOLUME_HIERARCHY_NODE_H
#define BOUNDING_VOLUME_HIERARCHY_NODE_H

#include "AxisAlignedBoundingBox.h"
#include "Hittable.h"
#include "HittableList.h"

class BoundingVolumeHierarchyNode : public Hittable
{
public:
	//There is a C++ minor detail here. This BoundingVolumeHierarchyNode(HittableList list) constructor creates an indirect
	//copy of the HittableList list. It then sends the m_objects member of this copy to another constructor
	//BoundingVolumeHierarchyNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end)
	//which modifies the m_objects member of this copy. As soon as this BoundingVolumeHierarchyNode(HittableList list) constructor
	//ends, the lifetime of this HittableList copy also ends. What we're left with is m_boundingBox, shared pointer to a BoundingVolumeHierarchyNode
	//representing left partition and shared pointer to a BoundingVolumeHierarchyNode representing right partition.
	BoundingVolumeHierarchyNode(HittableList list) : BoundingVolumeHierarchyNode(list.m_objects, 0, list.m_objects.size()) {}

	BoundingVolumeHierarchyNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end);

	virtual bool m_hit(const Ray& r, Interval rayInterval, HitRecord& record) const override;
	virtual AxisAlignedBoundingBox m_getBoundingBox() const override;
private:
	shared_ptr<Hittable> m_left; //BoundingVolumeHierarchyNode
	shared_ptr<Hittable> m_right; //BoundingVolumeHierarchyNode
	AxisAlignedBoundingBox m_boundingBox;

	static bool s_boxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisIndex);
	static bool s_boxXCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
	static bool s_boxYCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
	static bool s_boxZCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
};

#endif