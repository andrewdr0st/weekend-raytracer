#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include "util.h"

using color = vec3;

void writeColor(std::ostream& out, const color& pixelColor) {
    float r = pixelColor.x();
    float g = pixelColor.y();
    float b = pixelColor.z();

    static const Interval intensity(0.0, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif