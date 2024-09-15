#ifndef AABB_H
#define AABB_H

#include "interval.h"

class AABB {
    public:
        Interval x, y, z;

        AABB() {}

        AABB(const Interval& x, const Interval& y, const Interval& z): x(x), y(y), z(z) {
            padToMinimums();
        }

        AABB(const point3& a, const point3& b) {
            x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
            y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
            z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

            padToMinimums();
        }

        AABB(const AABB& a, const AABB& b) {
            x = Interval(a.x, b.x);
            y = Interval(a.y, b.y);
            z = Interval(a.z, b.z);
        }

        const Interval& axisInterval(int n) const {
            if (n == 1) {
                return y;
            }
            if (n == 2) {
                return z;
            }
            return x;
        }

        bool hit(const ray& r, Interval rayT) const {
            const point3& rayOrig = r.origin();
            const vec3& rayDir = r.direction();

            for (int axis = 0; axis < 3; axis++) {
                const Interval& ax = axisInterval(axis);
                const float adinv = 1.0 / rayDir[axis];

                float t0 = (ax.min - rayOrig[axis]) * adinv;
                float t1 = (ax.max - rayOrig[axis]) * adinv;

                if (t0 < t1) {
                    if (t0 > rayT.min) {
                        rayT.min = t0;
                    }
                    if (t1 < rayT.max) {
                        rayT.max = t1;
                    }
                } else {
                    if (t1 > rayT.min) {
                        rayT.min = t1;
                    }
                    if (t0 < rayT.max) {
                        rayT.max = t0;
                    }
                }

                if (rayT.max <= rayT.min) {
                    return false;
                }
            }

            return true;
        }

        int longestAxis() const {
            if (x.size() > y.size()) {
                return x.size() > z.size() ? 0 : 2;
            } else {
                return y.size() > z.size() ? 1 : 2;
            }
        }

        static const AABB empty, universe;
    
    private:
        void padToMinimums() {
            float delta = 0.0001;
            if (x.size() < delta) {
                x = x.expand(delta);
            }
            if (y.size() < delta) {
                y = y.expand(delta);
            }
            if (z.size() < delta) {
                z = z.expand(delta);
            }
        }
};

const AABB AABB::empty    = AABB(Interval::empty,    Interval::empty,    Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

#endif