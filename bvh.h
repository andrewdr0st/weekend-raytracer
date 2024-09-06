#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>

class BVHNode : public hittable {
    public:
        BVHNode(hittableList list): BVHNode(list.objects, 0, list.objects.size()) {}

        BVHNode(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end) {
            bbox = AABB::empty;

            for (size_t objectIndex = start; objectIndex < end; objectIndex++) {
                bbox = AABB(bbox, objects[objectIndex]->boundingBox());
            }
            
            int axis = bbox.longestAxis();

            auto comparator = (axis == 0) ? boxXCompare : (axis == 1) ? boxYCompare : boxZCompare;

            size_t objectSpan = end - start;

            if (objectSpan == 1) {
                left = right = objects[start];
            } else if (objectSpan == 2) {
                left = objects[start];
                right = objects[start + 1];
            } else {
                std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

                int mid = start + objectSpan / 2;
                left = make_shared<BVHNode>(objects, start, mid);
                right = make_shared<BVHNode>(objects, mid, end);
            }
        }

        bool hit(const ray& r, Interval rayT, hitRecord& rec) const override {
        if (!bbox.hit(r, rayT))
            return false;

        bool hitLeft = left->hit(r, rayT, rec);
        bool hitRight = right->hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

        return hitLeft || hitRight;
    }

    AABB boundingBox() const override { return bbox; }

    private:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        AABB bbox;

        static bool boxCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
            auto aAxisInterval = a->boundingBox().axisInterval(axis);
            auto bAxisInterval = b->boundingBox().axisInterval(axis);
            return aAxisInterval.min < bAxisInterval.min;
        }

        static bool boxXCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return boxCompare(a, b, 0);
        }

        static bool boxYCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return boxCompare(a, b, 1);
        }

        static bool boxZCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return boxCompare(a, b, 2);
        }
};

#endif