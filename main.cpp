#include "util.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"


color rayColor(const ray& r, const hittable& world) {
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

int main() {

    float aspectRatio = 16.0 / 9.0;

    int imageWidth = 600;
    int imageHeight = int(imageWidth / aspectRatio);
    imageHeight = imageHeight < 1 ? 1 : imageHeight;

    hittableList world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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

            color pixelColor = rayColor(r, world);
            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.                 \n";

    return 0;
}