#ifndef SPHERE_H
#define SPHERE_H

#include "util.h"
#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, float radius): center(center), radius(std::fmax(0,radius)) {}

        bool hit(const ray& r, Interval rayT, hitRecord& rec) const override {
            vec3 oc = center - r.origin();
            float a = r.direction().lengthSquared();
            float h = dot(r.direction(), oc);
            float c = oc.lengthSquared() - radius * radius;
            float discriminant = h * h - a * c;

            if (discriminant < 0) {
                return false;
            }

            float sqrtd = std::sqrt(discriminant);

            float root = (h - sqrtd) / a;
            if (!rayT.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!rayT.surrounds(root)) {
                    return false;
                }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outwardNormal = (rec.p - center) / radius;
            rec.setFaceNormal(r, outwardNormal);

            return true;
        }

    private:
        point3 center;
        float radius;
};

#endif