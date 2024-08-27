#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include "util.h"
#include "hittable.h"


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
        }

        bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
            hitRecord tempRec;
            bool hitAnything = false;
            float closestSoFar = rayT.max;

            for (const auto& obj : objects) {
                if (obj->hit(r, interval(rayT.min, closestSoFar), tempRec)) {
                    hitAnything = true;
                    closestSoFar = tempRec.t;
                    rec = tempRec;
                }
            }

            return hitAnything;
        }
};

#endif