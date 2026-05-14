#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

class camera {
  public:
    double aspect_ratio = 1.0;
    int image_width  = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;
    color background;

    double vfov = 90;
    point3 lookfrom = point3(0,0,0);
    point3 lookat = point3(0,0,-1);
    vec3 vup = vec3(0,1,0);

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable& world) {
        initialize();

        // Allocate framebuffer: one color per pixel
        std::vector<color> framebuffer(image_width * image_height);

        // Determine thread count
        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 4; // fallback
        // else num_threads-=4; //for my system

        std::clog << "Rendering with " << num_threads << " threads.\n";

        // Atomic counter for next row to process (dynamic work-stealing)
        std::atomic<int> next_row(0);
        std::atomic<int> rows_done(0);
        std::mutex log_mutex;

        auto worker = [&]() {
            while (true) {
                int j = next_row.fetch_add(1);
                if (j >= image_height) break;

                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    framebuffer[j * image_width + i] = pixel_samples_scale * pixel_color;
                }

                int done = rows_done.fetch_add(1) + 1;
                {
                    std::lock_guard<std::mutex> lock(log_mutex);
                    std::clog << "\rScanlines remaining: "
                              << (image_height - done) << ' ' << std::flush;
                }
            }
        };

        // Launch threads
        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        for (unsigned int t = 0; t < num_threads; t++) {
            threads.emplace_back(worker);
        }
        for (auto& th : threads) th.join();

        std::clog << "\rScanlines remaining: 0 \nDone.                 \n";

        // Write PPM header and pixels in order (single-threaded, fast)
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                write_color(std::cout, framebuffer[j * image_width + i]);
            }
        }
    }

  private:
    int    image_height;
    double pixel_samples_scale;
    point3 center;
    point3 pixel00_loc;
    vec3   pixel_delta_u;
    vec3   pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = unit_vector(cross(w, u));

        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        if (depth <= 0) return color(0,0,0);
        hit_record rec;

        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);

        return color_from_emission + color_from_scatter;
    }
};

#endif