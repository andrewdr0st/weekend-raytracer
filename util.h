#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


using std::make_shared;
using std::shared_ptr;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float deg2Rad(float degrees) {
    return degrees * pi / 180.0;
}

inline float randomFloat() {
    return std::rand() / (RAND_MAX / 1.0);
}

inline float randomFloat(float min, float max) {
    return min + (max - min) * randomFloat();
}


#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif