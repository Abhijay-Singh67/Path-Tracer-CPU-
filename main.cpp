#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "bvh.h"
#include "camera.h"

int main() {
    hittable_list world;

    // Ground
    auto ground_material =
        make_shared<lambertian>(color(0.8, 0.8, 0.0));

    world.add(
        make_shared<sphere>(
            point3(0, -100.5, -1),
            100,
            ground_material
        )
    );

    // Moving sphere
    auto moving_material =
        make_shared<lambertian>(color(0.1, 0.2, 0.9));

    point3 center1(0, 0, -1);
    point3 center2(0, 0.5, -1);

    world.add(
        make_shared<sphere>(
            center1,
            center2,
            0.5,
            moving_material
        )
    );

    // Metal sphere
    auto metal_material =
        make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);

    world.add(
        make_shared<sphere>(
            point3(1, 0, -1),
            0.5,
            metal_material
        )
    );

    // Glass sphere
    auto glass_material =
        make_shared<dielectric>(1.5);

    world.add(
        make_shared<sphere>(
            point3(-1, 0, -1),
            0.5,
            glass_material
        )
    );

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;

    // Lower for laptop
    cam.samples_per_pixel = 20;
    cam.max_depth = 10;

    cam.vfov = 40;
    cam.lookfrom = point3(3, 1.5, 2);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    // Optional depth of field
    cam.defocus_angle = 0.0;
    cam.focus_dist = 1.0;

    cam.render(world);

    return 0;
}