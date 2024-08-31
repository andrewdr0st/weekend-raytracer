#include "util.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"



int main() {
    hittableList world;

    shared_ptr<Material> groundMat = make_shared<Lambertian>(color(0.5, 0.6, 0.3));
    shared_ptr<Material> greenMat = make_shared<Lambertian>(color(0.25, 1.0, 0.25));
    shared_ptr<Material> blueMat = make_shared<Lambertian>(color(0.25, 0.25, 1.0));
    shared_ptr<Material> whiteMetal = make_shared<Metal>(color(0.9, 0.9, 0.9), 0.25);
    shared_ptr<Material> bronzeMetal = make_shared<Metal>(color(0.8, 0.4, 0.1), 0.6);

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, groundMat));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, blueMat));
    world.add(make_shared<sphere>(point3(-1.1, -0.425, -0.6), 0.1, greenMat));
    world.add(make_shared<sphere>(point3(-1.0, 0.25, -2.0), 0.75, whiteMetal));
    world.add(make_shared<sphere>(point3(1.25, 0, -1), 0.5, bronzeMetal));
    

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 600;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 20;

    camera.render(world);

    return 0;
}