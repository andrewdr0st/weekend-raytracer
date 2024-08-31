#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "hittable.h"

class Camera {
    public:
        float aspectRatio = 1.0;
        int imageWidth = 100;
        int samplesPerPixel = 10;
        int maxDepth = 10;

        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++) {
                std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                for (int i = 0; i < imageWidth; i++) {
                    color pixelColor(0, 0, 0);
                    for (int sample = 0; sample < samplesPerPixel; sample++) {
                        ray r = getRay(i, j);
                        pixelColor += rayColor(r, maxDepth, world);
                    }
                    writeColor(std::cout, pixelSamplesScale * pixelColor);
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
        float pixelSamplesScale;

        void initialize() {
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = imageHeight < 1 ? 1 : imageHeight;

            pixelSamplesScale = 1.0 / samplesPerPixel;

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

        ray getRay(int i, int j) const {
            vec3 offset = sampleSquare();
            vec3 pixelSample = upperLeftPixel + (i + offset.x()) * pixelDeltaU + (j + offset.y()) * pixelDeltaV;
            vec3 rayOrigin = center;
            vec3 rayDirection = pixelSample - rayOrigin;

            return ray(rayOrigin, rayDirection);
        }

        vec3 sampleSquare() const {
            return vec3(randomFloat() - 0.5, randomFloat() - 0.5, 0);
        }

        color rayColor(const ray& r, int depth, const hittable& world) const {
            static color sc = color(0.5, 0.5, 0.5);
            static color c1 = color(1.0, 0.35, 0.65);
            static color c2 = color(0.20, 0.40, 1.0);

            if (depth == 0) {
                return color(0, 0, 0);
            }

            hitRecord rec;
            if (world.hit(r, Interval(0.001, infinity), rec)) {
                vec3 dir = rec.normal + randomUnitVector();
                return 0.5 * rayColor(ray(rec.p, dir), depth - 1, world);
            }

            vec3 unitDir = unitVector(r.direction());
            float a = 0.5 * (unitDir.y() + 1.0);
            return (1.0 - a) * c1 + a * c2;
        }
};

#endif