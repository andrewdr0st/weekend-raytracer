#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include "util.h"
#include "hittable.h"
#include "aabb.h"

class hittableList : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittableList() {}
        hittableList(shared_ptr<hittable> object) {
            add(object);
        }

        void clear() {
            objects.clear();
        }

        void add(shared_ptr<hittable> obj) {
            objects.push_back(obj);
            bbox = AABB(bbox, obj->boundingBox());
        }

        bool hit(const ray& r, Interval rayT, hitRecord& rec) const override {
            hitRecord tempRec;
            bool hitAnything = false;
            float closestSoFar = rayT.max;

            for (const auto& obj : objects) {
                if (obj->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
                    hitAnything = true;
                    closestSoFar = tempRec.t;
                    rec = tempRec;
                }
            }

            return hitAnything;
        }

        AABB boundingBox() const override {
            return bbox;
        }

    private:
        AABB bbox;
};

#endif