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
#include "heterogeneous_medium.h"

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
    cam.image_width       = 600;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;
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

void dark_room(){

    hittable_list world;

    // =====================================================
    // MATERIALS
    // =====================================================

    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto floor_mat = make_shared<lambertian>(color(0.45, 0.42, 0.38));
    auto dark = make_shared<lambertian>(color(0.08, 0.08, 0.08));
    auto metal_mat = make_shared<metal>(color(0.88, 0.9, 0.92), 0.03);

    auto marble_tex = make_shared<noise_texture>(5);
    auto marble = make_shared<lambertian>(marble_tex);

    // Clear glass — tinting kills transmitted intensity
    auto window_glass = make_shared<dielectric>(1.5);

    // VERY bright warm sun — needs to overpower the small window aperture
    auto sunlight = make_shared<diffuse_light>(
        color(25, 22, 17), 1.0
    );

    // =====================================================
    // ROOM DIMENSIONS
    // =====================================================

    double room_w = 8;
    double room_h = 5;
    double room_d = 8;

    // =====================================================
    // ROOM (floor, ceiling, back, left, right)
    // =====================================================

    world.add(make_shared<quad>(point3(0,0,0),
        vec3(room_w,0,0), vec3(0,0,room_d), floor_mat));

    world.add(make_shared<quad>(point3(0,room_h,room_d),
        vec3(room_w,0,0), vec3(0,0,-room_d), white));

    world.add(make_shared<quad>(point3(0,0,room_d),
        vec3(room_w,0,0), vec3(0,room_h,0), white));

    world.add(make_shared<quad>(point3(0,0,0),
        vec3(0,0,room_d), vec3(0,room_h,0), white));

    world.add(make_shared<quad>(point3(room_w,0,room_d),
        vec3(0,0,-room_d), vec3(0,room_h,0), white));

    // =====================================================
    // FRONT WALL WITH WINDOW CUTOUT
    // =====================================================

    world.add(make_shared<quad>(point3(0,0,0),
        vec3(2.5,0,0), vec3(0,room_h,0), white));

    world.add(make_shared<quad>(point3(5.5,0,0),
        vec3(2.5,0,0), vec3(0,room_h,0), white));

    world.add(make_shared<quad>(point3(2.5,0,0),
        vec3(3,0,0), vec3(0,1.2,0), white));

    world.add(make_shared<quad>(point3(2.5,4,0),
        vec3(3,0,0), vec3(0,1,0), white));

    // Window glass
    world.add(make_shared<quad>(point3(2.5,1.2,0.01),
        vec3(3,0,0), vec3(0,2.8,0), window_glass));

    // =====================================================
    // SUNLIGHT — offset to one side so rays enter at an angle
    // =====================================================
    //
    // Placed off-axis (x shifted left, slightly above window center)
    // so the god rays slant across the room instead of going
    // straight in. Closer to the window = more intense rays.
    //

    world.add(make_shared<quad>(
        point3(-2.0, 3.0, -3.0),   // shifted left + up
        vec3(4, 0, 0),
        vec3(0, 3, 0),
        sunlight
    ));

    // =====================================================
    // OBJECTS INSIDE ROOM
    // =====================================================

    world.add(make_shared<sphere>(point3(5.8,1.0,5.3), 1.0, metal_mat));
    world.add(make_shared<sphere>(point3(2.2,0.7,4.0), 0.7, marble));

    shared_ptr<hittable> tall_box =
        box(point3(0,0,0), point3(1.3,3.2,1.3), dark);
    tall_box = make_shared<rotate_y>(tall_box, 20);
    tall_box = make_shared<translate>(tall_box, vec3(3.4,0,2.3));
    world.add(tall_box);

    // =====================================================
    // FOG — bounded inside the room, slightly denser
    // =====================================================
    //
    // Use a box-shaped boundary matching the room interior so
    // fog doesn't leak outside (which would block sunlight before
    // it reaches the window). Density bumped up a bit so shafts
    // are visible but the room isn't a milk bath.
    //

    auto fog_boundary = box(
        point3(0.05, 0.05, 0.05),
        point3(room_w - 0.05, room_h - 0.05, room_d - 0.05),
        make_shared<dielectric>(1.0)
    );

    world.add(make_shared<constant_medium>(
        fog_boundary,
        0.04,
        color(0.95, 0.95, 0.95)
    ));

    // =====================================================
    // BVH
    // =====================================================

    world = hittable_list(make_shared<bvh_node>(world));

    // =====================================================
    // CAMERA — INSIDE the room, looking toward window+objects
    // =====================================================

    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;   // fog + small light → needs more samples
    cam.max_depth = 20;
    cam.background = color(0,0,0);

    cam.vfov = 55;
    cam.lookfrom = point3(6.5, 2.2, 6.8);   // back-right corner, inside
    cam.lookat   = point3(3.0, 2.0, 1.0);   // toward window + box
    cam.vup = vec3(0,1,0);
    cam.defocus_angle = 0.0;

    cam.render(world);
}

void cornell_smoke_ball() {

    hittable_list world;

    // =====================================================
    // MATERIALS
    // =====================================================

    auto red    = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white  = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto blue   = make_shared<lambertian>(color(0.15, 0.12, 0.55));  // purple-blue
    auto light  = make_shared<diffuse_light>(color(12, 11, 10), 1.0); // slightly warm

    // =====================================================
    // CORNELL BOX (blue replaces green)
    // =====================================================

    // Right wall (blue)
    world.add(make_shared<quad>(point3(555,0,0),
        vec3(0,555,0), vec3(0,0,555), blue));

    // Left wall (red)
    world.add(make_shared<quad>(point3(0,0,0),
        vec3(0,555,0), vec3(0,0,555), red));

    // Ceiling light — small rectangular panel, centered
    world.add(make_shared<quad>(point3(213, 554, 227),
        vec3(130,0,0), vec3(0,0,105), light));

    // Ceiling
    world.add(make_shared<quad>(point3(0,555,0),
        vec3(555,0,0), vec3(0,0,555), white));

    // Floor
    world.add(make_shared<quad>(point3(0,0,0),
        vec3(555,0,0), vec3(0,0,555), white));

    // Back wall
    world.add(make_shared<quad>(point3(0,0,555),
        vec3(555,0,0), vec3(0,555,0), white));

    // =====================================================
    // CLOUD — multiple overlapping spheres for non-uniform look
    // =====================================================
    //
    // Big puffy top (denser) + smaller wispy tail below.
    // Stack/overlap a few spheres at varying densities so the
    // result reads as a cloud rather than a perfect ball of fog.
    //

    auto cloud_color = color(1, 1, 1);

    // Main upper puff (the dense "head" of the cloud)
    auto puff1 = make_shared<sphere>(point3(278, 340, 278), 110, white);
    world.add(make_shared<constant_medium>(puff1, 0.045, cloud_color));

    // Secondary lobe — offset slightly, gives the lumpy top edge
    auto puff2 = make_shared<sphere>(point3(330, 360, 260), 75, white);
    world.add(make_shared<constant_medium>(puff2, 0.05, cloud_color));

    auto puff3 = make_shared<sphere>(point3(230, 355, 290), 70, white);
    world.add(make_shared<constant_medium>(puff3, 0.05, cloud_color));

    // Middle body — thinner, transitions to tail
    auto puff4 = make_shared<sphere>(point3(278, 245, 278), 80, white);
    world.add(make_shared<constant_medium>(puff4, 0.025, cloud_color));

    // Wispy tail — very low density
    auto puff5 = make_shared<sphere>(point3(285, 160, 278), 60, white);
    world.add(make_shared<constant_medium>(puff5, 0.012, cloud_color));

    // =====================================================
    // AMBIENT ROOM HAZE — very thin, fills entire box
    // =====================================================
    //
    // Gives the slight overall mist visible in your reference
    // (haze near floor, soft falloff on walls).
    //

    auto room_boundary = box(
        point3(1, 1, 1),
        point3(554, 554, 554),
        make_shared<dielectric>(1.0)
    );

    world.add(make_shared<constant_medium>(
        room_boundary,
        0.0015,         // very thin — just enough to soften
        color(1, 1, 1)
    ));

    // =====================================================
    // BVH
    // =====================================================

    world = hittable_list(make_shared<bvh_node>(world));

    // =====================================================
    // CAMERA
    // =====================================================

    camera cam;
    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;   // volumetrics are noisy
    cam.max_depth         = 50;
    cam.background        = color(0, 0, 0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0, 1, 0);
    cam.defocus_angle = 0;

    cam.render(world);

}

void cornell_cloud() {
    hittable_list world;

    // =====================================================
    // MATERIALS
    // =====================================================

    auto red    = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white  = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto blue   = make_shared<lambertian>(color(0.15, 0.12, 0.55));
    auto light  = make_shared<diffuse_light>(color(12, 11, 10), 1.0);

    // =====================================================
    // CORNELL BOX
    // =====================================================

    world.add(make_shared<quad>(point3(555,0,0),
        vec3(0,555,0), vec3(0,0,555), blue));

    world.add(make_shared<quad>(point3(0,0,0),
        vec3(0,555,0), vec3(0,0,555), red));

    world.add(make_shared<quad>(point3(213, 554, 227),
        vec3(130,0,0), vec3(0,0,105), light));

    world.add(make_shared<quad>(point3(0,555,0),
        vec3(555,0,0), vec3(0,0,555), white));

    world.add(make_shared<quad>(point3(0,0,0),
        vec3(555,0,0), vec3(0,0,555), white));

    world.add(make_shared<quad>(point3(0,0,555),
        vec3(555,0,0), vec3(0,555,0), white));

    // =====================================================
    // HETEROGENEOUS CLOUD
    // =====================================================
    //
    // Shape: blob centered ~(278, 320, 278), denser at top,
    // tapering into a wispy tail below. Built from:
    //
    //   (1) base shape: gaussian falloff from a moving center
    //       — center shifts upward as we go up, creating a
    //         bulbous "head" + thinning "neck/tail"
    //   (2) vertical density gradient: more mass up top
    //   (3) noise-like modulation via cheap trig: gives the
    //       lumpy non-uniform feel without needing perlin
    //
    // d_max set to ~0.06 (function peaks near there).
    //

    auto cloud_density = [](const point3& p) {
        // --- Base shape: vertically-stretched blob with a tail ---
        // Move the "center" point upward as p.y increases,
        // so cross-sections higher up are wider/denser.

        double cx = 278.0;
        double cz = 278.0;
        double cy = 320.0;

        // Lateral distance from vertical axis
        double dx = p.x() - cx;
        double dz = p.z() - cz;
        double r2 = dx*dx + dz*dz;

        // Vertical position normalized: 1 at top of cloud, 0 at bottom
        double h = (p.y() - 130.0) / 250.0;   // ranges roughly 0..1.5
        if (h < 0.0 || h > 1.5) return 0.0;

        // Radius of the cloud at this height — fat at top, thin at bottom
        double radius = 30.0 + 90.0 * h;       // grows with height
        double radial_falloff = std::exp(-r2 / (radius * radius));

        // Vertical envelope: peak near h ~ 1.0, fall off above and below
        double vert = std::exp(-(h - 1.0) * (h - 1.0) / 0.25);

        // --- Lumpiness: cheap pseudo-noise via stacked sines ---
        // Gives the cloud a non-uniform, puffy surface without
        // needing a real perlin implementation.
        double lump =
              0.5 + 0.5 * std::sin(p.x() * 0.035)
                        * std::cos(p.y() * 0.04)
                        * std::sin(p.z() * 0.03);
        lump = 0.6 + 0.7 * lump;   // remap to ~[0.6, 1.3]

        double sigma = 0.06 * radial_falloff * vert * lump;

        // Safety clamp (lump can push us slightly above 0.06)
        if (sigma > 0.06) sigma = 0.06;
        if (sigma < 0.0)  sigma = 0.0;
        return sigma;
    };

    // Boundary: generous AABB around where the cloud lives.
    // Tighter = fewer wasted Woodcock steps.
    auto cloud_boundary = box(
        point3(140, 130, 140),
        point3(420, 470, 420),
        make_shared<dielectric>(1.0)   // material unused by medium
    );

    world.add(make_shared<heterogeneous_medium>(
        cloud_boundary,
        0.0,      // d_min
        0.06,     // d_max  — must be ≥ peak of cloud_density
        cloud_density,
        color(1, 1, 1)
    ));

    // =====================================================
    // AMBIENT ROOM HAZE (kept as constant_medium — uniform)
    // =====================================================

    auto room_boundary = box(
        point3(1, 1, 1),
        point3(554, 554, 554),
        make_shared<dielectric>(1.0)
    );

    world.add(make_shared<constant_medium>(
        room_boundary,
        0.0015,
        color(1, 1, 1)
    ));

    // =====================================================
    // BVH
    // =====================================================

    world = hittable_list(make_shared<bvh_node>(world));

    // =====================================================
    // CAMERA
    // =====================================================

    camera cam;
    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 400;   // heterogeneous = noisier, bump it
    cam.max_depth         = 50;
    cam.background        = color(0, 0, 0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0, 1, 0);
    cam.defocus_angle = 0;

    cam.render(world);
}

int main(){
    switch (12) {
        case 1: glass_sphere(); break;
        case 2: earth(); break;
        case 3: perlin_spheres(); break;
        case 4: quads(); break;
        case 5: simple_light(); break;
        case 6: cornell_box(); break;
        case 7: cornell_smoke(); break;
        case 8: final_scene(); break;
        case 9: fun_scene(); break;
        case 10: dark_room(); break;
        case 11: cornell_smoke_ball(); break;
        case 12: cornell_cloud(); break;
    }
    return 0;
}