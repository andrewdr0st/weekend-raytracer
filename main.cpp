#include "util.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"




int main() {
    hittableList world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 600;
    camera.samplesPerPixel = 100;

    camera.render(world);

    return 0;
}