#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "hittable.h"
#include "material.h"

class Camera {
    public:
        float aspectRatio = 1.0;
        int imageWidth = 100;
        int samplesPerPixel = 10;
        int maxDepth = 10;
        color background;

        float fov = 90;
        point3 lookFrom = point3(0, 0, 0);
        point3 lookAt = point3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);

        float defocusAngle = 0;
        float focusDist = 10;

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
        vec3 u, v, w;
        vec3 defocusDiskU;
        vec3 defocusDistV;

        void initialize() {
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = imageHeight < 1 ? 1 : imageHeight;

            pixelSamplesScale = 1.0 / samplesPerPixel;

            center = lookFrom;

            float theta = deg2Rad(fov);
            float h = std::tan(theta / 2);
            float viewportHeight = 2.0 * h * focusDist;
            float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);

            w = unitVector(lookFrom - lookAt);
            u = unitVector(cross(vup, w));
            v = cross(w, u);

            vec3 viewportU = viewportWidth * u;
            vec3 viewportV = viewportHeight * -v;

            pixelDeltaU = viewportU / imageWidth;
            pixelDeltaV = viewportV / imageHeight;

            vec3 viewportUpperLeft = center - focusDist * w - viewportU / 2 - viewportV / 2;
            upperLeftPixel = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

            float defocusRadius = focusDist * std::tan(deg2Rad(defocusAngle / 2));
            defocusDiskU = u * defocusRadius;
            defocusDistV = v * defocusRadius;
        }

        ray getRay(int i, int j) const {
            vec3 offset = sampleSquare();
            vec3 pixelSample = upperLeftPixel + (i + offset.x()) * pixelDeltaU + (j + offset.y()) * pixelDeltaV;
            vec3 rayOrigin = defocusAngle <= 0 ? center : defocusDiskSample();
            vec3 rayDirection = pixelSample - rayOrigin;

            return ray(rayOrigin, rayDirection);
        }

        vec3 sampleSquare() const {
            return vec3(randomFloat() - 0.5, randomFloat() - 0.5, 0);
        }

        vec3 defocusDiskSample() const {
            vec3 p = randomInUnitDisk();
            return center + p[0] * defocusDiskU + p[1] * defocusDistV;
        }

        color rayColor(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0) {
                return color(0, 0, 0);
            }

            hitRecord rec;
            
            if (!world.hit(r, Interval(0.001, infinity), rec)) {
                return background;
            }

            ray scattered;
            color attenuation;
            color colorFromEmmision = rec.mat->emitted(rec.u, rec.v, rec.p);

            if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
                return colorFromEmmision;
            }

            color colorFromScatter = attenuation * rayColor(scattered, depth - 1, world);

            return colorFromEmmision + colorFromScatter;
        }
};

#endif