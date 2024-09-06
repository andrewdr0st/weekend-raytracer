#ifndef INTERVAL_H
#define INTERVAL_H

#include "util.h"

class Interval {
  public:
    float min, max;

    Interval() : min(+infinity), max(-infinity) {}

    Interval(float min, float max): min(min), max(max) {}

    Interval(const Interval& a, const Interval& b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    float size() const {
        return max - min;
    }

    bool contains(float x) const {
        return min <= x && x <= max;
    }

    bool surrounds(float x) const {
        return min < x && x < max;
    }

    float clamp(float x) const {
        if (x < min) {
            return min;
        } else if (x > max) {
            return max;
        } else {
            return x;
        }
    }

    Interval expand(float delta) const {
        float padding = delta / 2;
        return Interval(min - padding, max + padding);
    }

    static const Interval empty, universe;
};

const Interval Interval::empty    = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif