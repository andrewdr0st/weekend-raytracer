#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "util.h"

using color = vec3;

void writeColor(std::ostream& out, const color& pixelColor) {
    float r = pixelColor.x();
    float g = pixelColor.y();
    float b = pixelColor.z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif