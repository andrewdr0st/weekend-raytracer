#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "hittable.h"

class Camera {
    public:
        float aspectRatio = 16.0 / 9.0;
        int imageWidth = 600;

        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++) {
                std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                for (int i = 0; i < imageWidth; i++) {
                    vec3 pixelCenter = upperLeftPixel + (i * pixelDeltaU) + (j * pixelDeltaV);
                    vec3 rayDir = pixelCenter - center;
                    ray r = ray(center, rayDir);

                    color pixelColor = rayColor(r, world);
                    writeColor(std::cout, pixelColor);
                }
            }

            std::clog << "\rDone.                 \n";
        }

    private:
        int imageHeight;
        point3 center;
        point3 upperLeftPixel;
        vec3 pixelDeltaU;
        vec3 pixelDeltaV;

        void initialize() {
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = imageHeight < 1 ? 1 : imageHeight;

            float viewportHeight = 2.0;
            float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);
            float focalLength = 1.0;

            center = point3(0, 0, 0);

            vec3 viewportU = vec3(viewportWidth, 0, 0);
            vec3 viewportV = vec3(0, -viewportHeight, 0);

            pixelDeltaU = viewportU / imageWidth;
            pixelDeltaV = viewportV / imageHeight;

            vec3 viewportUpperLeft = center - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
            upperLeftPixel = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
        }

        color rayColor(const ray& r, const hittable& world) const {
            color sc = color(1.0, 0.75, 0.25);
            color c1 = color(1.0, 0.25, 0.75);
            color c2 = color(0.25, 0.50, 1.0);

            hitRecord rec;
            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + sc);
            }

            vec3 unitDir = unitVector(r.direction());
            float a = 0.5 * (unitDir.y() + 1.0);
            return (1.0 - a) * c1 + a * c2;
        }
};

#endif