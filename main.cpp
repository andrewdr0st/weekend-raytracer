#include "util.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"



int main() {
    hittableList world;

    shared_ptr<Material> groundMat = make_shared<Lambertian>(color(0.5, 0.6, 0.3));
    shared_ptr<Material> redMat = make_shared<Lambertian>(color(1.0, 0.25, 0.25));
    shared_ptr<Material> greenMat = make_shared<Lambertian>(color(0.25, 1.0, 0.25));
    shared_ptr<Material> blueMat = make_shared<Lambertian>(color(0.25, 0.25, 1.0));
    shared_ptr<Material> whiteMetal = make_shared<Metal>(color(0.9, 0.9, 0.9), 0.25);
    shared_ptr<Material> bronzeMetal = make_shared<Metal>(color(0.8, 0.4, 0.1), 0.6);
    shared_ptr<Material> glass = make_shared<Dielectric>(1.5);

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, groundMat));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, blueMat));
    world.add(make_shared<sphere>(point3(-1.1, -0.4, -0.8), 0.1, greenMat));
    world.add(make_shared<sphere>(point3(-1.75, -0.3, -2.25), 0.2, redMat));
    world.add(make_shared<sphere>(point3(3.0, 0.15, -4.0), 0.75, whiteMetal));
    world.add(make_shared<sphere>(point3(1.25, 0, -0.9), 0.5, bronzeMetal));
    world.add(make_shared<sphere>(point3(-1.75, 0.05, -1.5), 0.6, glass));
    

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 600;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 20;
    camera.fov = 80;
    camera.lookFrom = point3(-1.5, 1.5, 0.5);
    camera.lookAt = point3(0, 0, -1);
    camera.vup = point3(0, 1, 0);
    camera.defocusAngle = 0.5;
    camera.focusDist = 10;

    camera.render(world);

    return 0;
}