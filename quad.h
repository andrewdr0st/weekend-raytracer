#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class Quad : public hittable {
    public:
        Quad(const point3& q, const vec3& u, const vec3& v, shared_ptr<Material> m): q(q), u(u), v(v), mat(m) {
            vec3 n = cross(u, v);
            normal = unitVector(n);
            d = dot(normal, q);
            w = n / dot(n, n);

            setBoundingBox();
        }

        virtual void setBoundingBox() {
            AABB boxDiag1 = AABB(q, q + u + v);
            AABB boxDiag2 = AABB(q + u, q + v);
            bbox = AABB(boxDiag1, boxDiag2);
        }
        
        AABB boundingBox() const override {
            return bbox;
        }

        bool hit(const ray& r, Interval rayT, hitRecord& rec) const override {
            float denom = dot(normal, r.direction());

            if (std::fabs(denom) < 0.00001) {
                return false;
            }

            float t = (d - dot(normal, r.origin())) / denom;
            if (!rayT.contains(t)) {
                return false;
            }

            point3 intersection = r.at(t);
            vec3 planarHitPoint = intersection - q;
            float a = dot(w, cross(planarHitPoint, v));
            float b = dot(w, cross(u, planarHitPoint));

            if (!isInterior(a, b, rec)) {
                return false;
            }

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.setFaceNormal(r, normal);

            return true;
        }

        virtual bool isInterior(float a, float b, hitRecord& rec) const {
            Interval unitInterval = Interval(0, 1);

            if (!unitInterval.contains(a) || !unitInterval.contains(b)) {
                return false;
            }

            rec.u = a;
            rec.v = b;
            return true;
        }

    private:
        point3 q;
        vec3 u, v;
        vec3 w;
        shared_ptr<Material> mat;
        AABB bbox;
        vec3 normal;
        float d;
};

#endif