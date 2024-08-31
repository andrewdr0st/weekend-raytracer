#ifndef VEC3_H
#define VEC3_H

#include "util.h"

class vec3 {
    public:
        float e[3];

        vec3(): e{0, 0, 0} {}
        vec3(float x, float y, float z): e{x, y, z} {}

        float x() const { return e[0]; }
        float y() const { return e[1]; }
        float z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        float operator[](int i) const { return e[i]; }
        float& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(float t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(float t) {
            return *this *= 1 / t;
        }

        float length() const {
            return std::sqrt(lengthSquared());
        }

        float lengthSquared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        static vec3 random() {
            return vec3(randomFloat(), randomFloat(), randomFloat());
        }

        static vec3 random(float min, float max) {
            return vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
        }
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, float t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, float t) {
    return (1 / t) * v;
}

inline float dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]
    );
}

inline vec3 unitVector(const vec3& v) {
    return v / v.length();
}

inline vec3 randomInUnitSphere() {
    while (true) {
        vec3 p = vec3::random(-1, 1);
        if (p.lengthSquared() < 1) {
            return p;
        }
    }
}

inline vec3 randomUnitVector() {
    return unitVector(randomInUnitSphere());
}

inline vec3 randomOnHemisphere(const vec3& normal) {
    vec3 v = randomUnitVector();
    if (dot(v, normal) > 0.0) {
        return v;
    } else {
        return -v;
    }
}

#endif