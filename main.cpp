#include "util.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "bvh.h"
#include "texture.h"


void lottaBalls() {
    hittableList world;

    shared_ptr<Texture> groundTex = make_shared<CheckerTexture>(0.5, color(0.1, 0.1, 0.3), color(0.8, 0.8, 0.9));
    shared_ptr<Material> groundMat = make_shared<Lambertian>(groundTex);
    shared_ptr<Material> redMat = make_shared<Lambertian>(color(1.0, 0.25, 0.25));
    shared_ptr<Material> greenMat = make_shared<Lambertian>(color(0.25, 1.0, 0.25));
    shared_ptr<Material> blueMat = make_shared<Lambertian>(color(0.25, 0.25, 1.0));
    shared_ptr<Material> whiteMetal = make_shared<Metal>(color(0.9, 0.9, 0.9), 0.25);
    shared_ptr<Material> bronzeMetal = make_shared<Metal>(color(0.8, 0.4, 0.1), 0.6);
    shared_ptr<Material> glass = make_shared<Dielectric>(1.5);

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, groundMat));
    world.add(make_shared<sphere>(point3(-3.1, 1.0, -1.0), 1.0, blueMat));
    world.add(make_shared<sphere>(point3(-0.75, 0.5, 2.0), 0.5, greenMat));
    world.add(make_shared<sphere>(point3(-1.95, 0.75, -4.25), 0.75, redMat));
    world.add(make_shared<sphere>(point3(1.25, 0.75, 3.0), 0.75, whiteMetal));
    world.add(make_shared<sphere>(point3(6, 1.75, -4.5), 1.75, bronzeMetal));
    world.add(make_shared<sphere>(point3(-0.25, 1.25, -0.25), 1.25, glass));

    for (int z = -10; z <= 6; z++) {
        int w = 4 + (6 - z) / 1.5;
        for (int x = -w; x <= w; x++) {
            float m = randomFloat();
            float r = randomFloat(0.1, 0.3);
            point3 pos = point3(x + randomFloat(-0.75, 0.75), r, z + randomFloat(-0.75, 0.75));
            shared_ptr<Material> mat;
            if (m < 0.65) {
                mat = make_shared<Lambertian>(color::random(0.25, 1.0));
            } else if (m < 0.95) {
                mat = make_shared<Metal>(color::random(0.5, 1.0), randomFloat(0.1, 0.75));
            } else {
                mat = make_shared<Dielectric>(randomFloat(1.3, 1.7));
            }
            world.add(make_shared<sphere>(pos, r, mat));
        }
    }

    world = hittableList(make_shared<BVHNode>(world));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 10;
    camera.maxDepth = 10;
    camera.fov = 70;
    camera.lookFrom = point3(0, 3, 5);
    camera.lookAt = point3(0, 0, 0);
    camera.vup = point3(0, 1, 0);
    camera.defocusAngle = 0.5;
    camera.focusDist = 6;
    camera.background = color(0.85, 0.7, 0.95);

    camera.render(world);
}

void doubleCheck() {
    hittableList world;

    shared_ptr<Texture> cTex = make_shared<CheckerTexture>(0.5, color(0.1, 0.1, 0.3), color(0.8, 0.8, 0.9));
    shared_ptr<Material> cMat = make_shared<Lambertian>(cTex);

    world.add(make_shared<sphere>(point3(0, -10, -3), 10.0, cMat));
    world.add(make_shared<sphere>(point3(0, 10, -3), 10.0, cMat));

    world = hittableList(make_shared<BVHNode>(world));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 10;
    camera.maxDepth = 10;
    camera.fov = 90;
    camera.lookFrom = point3(0, 0.5, 2);
    camera.lookAt = point3(0, 0, -3);
    camera.vup = point3(0, 1, 0);
    camera.defocusAngle = 0.2;
    camera.focusDist = 5;
    camera.background = color(0.85, 0.7, 0.95);

    camera.render(world);
}

void yoNotch() {
    auto imageTexture = make_shared<ImageTexture>("epicpoken.jpg");
    auto ballTex = make_shared<Lambertian>(imageTexture);
    auto ball = make_shared<sphere>(point3(0, 2, 4), 2, ballTex);
    auto noiseTexture = make_shared<NoiseTexture>(4);
    auto ballyTex = make_shared<Lambertian>(noiseTexture);
    auto bally = make_shared<sphere>(point3(0, -200, 0), 200, ballyTex);
    auto bingus = make_shared<sphere>(point3(0, 3, -5), 3, ballyTex);

    hittableList world;
    world.add(ball);
    world.add(bally);
    world.add(bingus);
    world = hittableList(make_shared<BVHNode>(world));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 10;
    camera.maxDepth = 10;
    camera.fov = 90;
    camera.lookFrom = point3(5, 2.5, 0);
    camera.lookAt = point3(0, 1, 0);
    camera.vup = point3(0, 1, 0);
    camera.defocusAngle = 0.2;
    camera.focusDist = 5;
    camera.background = color(0.85, 0.7, 0.95);

    camera.render(world);
}

void quadrilaterals() {
    hittableList world;

    shared_ptr<Material> orange = make_shared<Lambertian>(color(1.0, 0.5, 0.1));
    shared_ptr<Material> yellow = make_shared<Lambertian>(color(1.0, 1.0, 0.2));
    shared_ptr<Material> cyan = make_shared<Lambertian>(color(0.1, 1.0, 1.0));
    shared_ptr<Material> purple = make_shared<Lambertian>(color(0.5, 0.1, 1.0));
    shared_ptr<Material> magenta = make_shared<Lambertian>(color(1.0, 0.2, 1.0));

    world.add(make_shared<Quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), purple));
    world.add(make_shared<Quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), cyan));
    world.add(make_shared<Quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), magenta));
    world.add(make_shared<Quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), yellow));
    world.add(make_shared<Quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), orange));

    world = hittableList(make_shared<BVHNode>(world));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 20;
    camera.maxDepth = 10;
    camera.fov = 90;
    camera.lookFrom = point3(0, 1.0, 8);
    camera.lookAt = point3(0, 0, 0);
    camera.vup = point3(0, 1, 0);
    camera.defocusAngle = 0.3;
    camera.focusDist = 8;
    camera.background = color(0.85, 0.7, 0.95);

    camera.render(world);
}

void litFam() {
    hittableList world;

    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(color(1, 1, 1));
    auto difflightr = make_shared<DiffuseLight>(color(1, 0.3, 0.3));
    world.add(make_shared<Quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));
    world.add(make_shared<sphere>(point3(0, 7, 0), 2, difflightr));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 20;
    cam.background = color(0, 0, 0);
    cam.fov = 20;
    cam.lookFrom = point3(26, 3, 6);
    cam.lookAt = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocusAngle = 0;

    cam.render(world);
}

int main() {
    switch(5) {
        case 1: lottaBalls(); break;
        case 2: doubleCheck(); break;
        case 3: yoNotch(); break;
        case 4: quadrilaterals(); break;
        case 5: litFam(); break;
    }

    return 0;
}