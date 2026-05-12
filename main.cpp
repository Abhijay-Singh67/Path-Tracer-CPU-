#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "bvh.h"
#include "camera.h"
#include "texture.h"

void glass_sphere() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0,1,-1), 0.5, make_shared<dielectric>(1.55)));

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
}

void earth() {
    auto earth_texture = make_shared<image_texture>("external/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

int main(){
    switch (2) {
        case 1: glass_sphere(); break;
        case 2: earth(); break;
    }
    return 0;
}