#ifndef SPHERE_H
#define SPHERE_H

#include "util.h"
#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, float radius, shared_ptr<Material> m): center(center), radius(std::fmax(0, radius)), mat(m) {
            vec3 rvec = vec3(radius, radius, radius);
            bbox = AABB(center - rvec, center + rvec);
        }

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
            getSphereUV(outwardNormal, rec.u, rec.v);
            rec.mat = mat;

            return true;
        }

        AABB boundingBox() const override {
            return bbox;
        }

    private:
        point3 center;
        float radius;
        shared_ptr<Material> mat;
        AABB bbox;

        static void getSphereUV(const point3& p, float& u, float& v) {
            float theta = std::acos(-p.y());
            float phi = std::atan2(-p.z(), p.x()) + pi;

            u = phi / (2 * pi);
            v = theta / pi;
        }
};

#endif