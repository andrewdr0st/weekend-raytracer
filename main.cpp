#include <iostream>

#include "vec3.h"
#include "color.h"
#include "ray.h"

color rayColor(const ray& r) {
    color c1 = color(1.0, 0.25, 0.75);
    color c2 = color(0.25, 0.50, 1.0);

    vec3 unitDir = unitVector(r.direction());
    float a = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - a) * c1 + a * c2;
}

int main() {

    float aspectRatio = 16.0 / 9.0;

    int imageWidth = 600;
    int imageHeight = int(imageWidth / aspectRatio);
    imageHeight = imageHeight < 1 ? 1 : imageHeight;

    float viewportHeight = 2.0;
    float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);
    float focalLength = 1.0;
    vec3 cameraCenter = point3(0, 0, 0);

    vec3 viewportU = vec3(viewportWidth, 0, 0);
    vec3 viewportV = vec3(0, -viewportHeight, 0);

    vec3 pixelDeltaU = viewportU / imageWidth;
    vec3 pixelDeltaV = viewportV / imageHeight;

    vec3 viewportUpperLeft = cameraCenter - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
    vec3 upperLeftPixel = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

     std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            vec3 pixelCenter = upperLeftPixel + (i * pixelDeltaU) + (j * pixelDeltaV);
            vec3 rayDir = pixelCenter - cameraCenter;
            ray r = ray(cameraCenter, rayDir);

            color pixelColor = rayColor(r);
            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.                 \n";

    return 0;
}