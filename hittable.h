#ifndef HITTABLE_H
#define HITTABLE_H

#include "util.h"
#include "aabb.h"

class Material;

class hitRecord {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<Material> mat;
        float t;
        bool frontFace;

        void setFaceNormal(const ray& r, const vec3& outwardNormal) {
            frontFace = dot(r.direction(), outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, Interval rayT, hitRecord& rec) const = 0;

        virtual AABB boundingBox() const = 0;
};

#endif