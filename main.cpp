#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "bvh.h"
#include "camera.h"
#include "texture.h"
#include "constant_medium.h"

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
    cam.background = color(0.70, 0.80, 1.00);

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
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void perlin_spheres() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<dielectric>(1.55)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<annular_disc>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4),2, lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simple_light() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<dielectric>(1.55)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4), 5.0);
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_box() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15), 1.0);

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_smoke() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7), 1.0);

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 20;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void final_scene(){
    hittable_list world;

    auto checker = make_shared<checker_texture>(
        0.5,
        color(0.15, 0.15, 0.15),
        color(0.85, 0.85, 0.85)
    );

    auto floor_mat = make_shared<lambertian>(checker);

    auto marble = make_shared<noise_texture>(4);

    auto white = make_shared<lambertian>(
        color(0.8, 0.8, 0.8)
    );

    auto metal_mat = make_shared<metal>(
        color(0.8, 0.85, 0.9),
        0.05
    );

    auto glass = make_shared<dielectric>(1.5);

    auto light = make_shared<diffuse_light>(
        color(7, 6, 5), 1.0
    );

    // =====================================================
    // FLOOR
    // =====================================================

    world.add(make_shared<quad>(
        point3(-10, 0, -10),
        vec3(20, 0, 0),
        vec3(0, 0, 20),
        floor_mat
    ));

    // =====================================================
    // MARBLE SPHERE
    // =====================================================

    world.add(make_shared<sphere>(
        point3(0, 1, 0),
        1.0,
        make_shared<lambertian>(marble)
    ));

    // =====================================================
    // GLASS SPHERE
    // =====================================================

    world.add(make_shared<sphere>(
        point3(-2.2, 1, 0),
        1.0,
        glass
    ));

    // =====================================================
    // METAL SPHERE
    // =====================================================

    world.add(make_shared<sphere>(
        point3(2.2, 1, 0),
        1.0,
        metal_mat
    ));

    // =====================================================
    // GLOWING ANNULAR DISC LIGHT
    // =====================================================

    world.add(make_shared<annular_disc>(
        point3(-1.5, 5, -1.5),
        vec3(3, 0, 0),
        vec3(0, 0, 3),
        0.55,
        light
    ));

    // =====================================================
    // SMOKE VOLUME AROUND MARBLE SPHERE
    // =====================================================

    auto smoke_boundary = make_shared<sphere>(
        point3(0, 1, 0),
        1.2,
        glass
    );

    world.add(make_shared<constant_medium>(
        smoke_boundary,
        0.35,
        color(0.8, 0.85, 1.0)
    ));

    // =====================================================
    // ROTATED BOX
    // =====================================================

    shared_ptr<hittable> tall_box =
        box(
            point3(0,0,0),
            point3(1.5,3,1.5),
            white
        );

    tall_box = make_shared<rotate_y>(tall_box, 25);
    tall_box = make_shared<translate>(
        tall_box,
        vec3(4,0,-2)
    );

    world.add(tall_box);

    // =====================================================
    // SMALL EMISSIVE DISC
    // =====================================================

    world.add(make_shared<disc>(
        point3(-4, 3, -3),
        vec3(0.8, 0, 0),
        vec3(0, 0.8, 0),
        make_shared<diffuse_light>(
            color(3, 1.5, 1.5), 1.0
        )
    ));

    // =====================================================
    // GLOBAL ATMOSPHERIC FOG
    // =====================================================

    auto fog_boundary = make_shared<sphere>(
        point3(0,0,0),
        1000,
        glass
    );

    world.add(make_shared<constant_medium>(
        fog_boundary,
        0.0008,
        color(0.7, 0.8, 1.0)
    ));

    // =====================================================
    // BVH
    // =====================================================

    world = hittable_list(
        make_shared<bvh_node>(world)
    );

    // =====================================================
    // CAMERA
    // =====================================================

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 40;

    cam.background = color(0.02, 0.02, 0.03);

    cam.vfov     = 35;
    cam.lookfrom = point3(8, 3, 10);
    cam.lookat   = point3(0, 1, 0);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0.4;
    cam.focus_dist    = 10.0;

    // =====================================================
    // RENDER
    // =====================================================

    cam.render(world);
}

void fun_scene(){
    hittable_list world;

    // =====================================================
    // MATERIALS
    // =====================================================

    auto checker = make_shared<checker_texture>(
        0.5,
        color(0.1, 0.1, 0.1),
        color(0.9, 0.9, 0.9)
    );

    auto floor_mat = make_shared<lambertian>(checker);

    auto metal_mat = make_shared<metal>(
        color(0.92, 0.92, 0.95),
        0.02
    );

    // =====================================================
    // COLORED GLASS
    // =====================================================
    //
    // Blue-tinted dielectric.
    // Light passing through loses red/green energy.
    //

    auto blue_glass = make_shared<dielectric>(
        1.5,
        color(0.25, 0.55, 1.0)
    );

    // =====================================================
    // LIGHT
    // =====================================================

    auto light = make_shared<diffuse_light>(
        color(20, 20, 20), 5.0
    );

    // =====================================================
    // FLOOR
    // =====================================================

    world.add(make_shared<quad>(
        point3(-8, 0, -8),
        vec3(16, 0, 0),
        vec3(0, 0, 16),
        floor_mat
    ));

    // =====================================================
    // METAL SPHERE
    // =====================================================

    world.add(make_shared<sphere>(
        point3(0, 1, 0),
        1.0,
        metal_mat
    ));

    // =====================================================
    // COLORED GLASS PANEL
    // =====================================================
    //
    // Positioned directly between light and sphere.
    //

    world.add(make_shared<quad>(
        point3(-1.5, 0.0, 2.5),
        vec3(3.0, 0, 0),
        vec3(0, 3.0, 0),
        blue_glass
    ));

    // =====================================================
    // LIGHT SOURCE
    // =====================================================
    //
    // Small bright emissive sphere behind glass.
    //

    world.add(make_shared<sphere>(
        point3(0, 2.0, 5.5),
        0.45,
        light
    ));

    // =====================================================
    // LIGHT BLOCKERS
    // =====================================================
    //
    // These ensure light mostly reaches the sphere
    // ONLY through the colored glass.
    //

    auto black = make_shared<lambertian>(
        color(0.01, 0.01, 0.01)
    );

    // Left wall
    world.add(make_shared<quad>(
        point3(-6.0, 0, 1.5),
        vec3(4.5, 0, 0),
        vec3(0, 5, 0),
        black
    ));

    // Right wall
    world.add(make_shared<quad>(
        point3(1.5, 0, 1.5),
        vec3(4.5, 0, 0),
        vec3(0, 5, 0),
        black
    ));

    // Ceiling blocker
    world.add(make_shared<quad>(
        point3(-1.5, 3.1, 1.5),
        vec3(3.0, 0, 0),
        vec3(0, 0, 5.0),
        black
    ));

    // =====================================================
    // BVH
    // =====================================================

    world = hittable_list(
        make_shared<bvh_node>(world)
    );

    // =====================================================
    // CAMERA
    // =====================================================

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1280;

    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;

    cam.background = color(0,0,0);

    cam.vfov     = 35;

    cam.lookfrom = point3(8, 2.5, 0);
    cam.lookat   = point3(0, 1.2, 0);

    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.0;

    // =====================================================
    // RENDER
    // =====================================================

    cam.render(world);
}

int main(){
    switch (9) {
        case 1: glass_sphere(); break;
        case 2: earth(); break;
        case 3: perlin_spheres(); break;
        case 4: quads(); break;
        case 5: simple_light(); break;
        case 6: cornell_box(); break;
        case 7: cornell_smoke(); break;
        case 8: final_scene(); break;
        case 9: fun_scene(); break;
    }
    return 0;
}