#ifndef HETEROGENEOUS_MEDIUM_H
#define HETEROGENEOUS_MEDIUM_H

#include "hittable.h"
#include<functional>
#include "texture.h"
#include "material.h"

class heterogeneous_medium : public hittable {
    public:
        heterogeneous_medium(shared_ptr<hittable> boundary,double density_min, double density_max, std::function<double(const point3&)> density_fn, shared_ptr<texture> tex)
        : boundary(boundary),d_min(density_min), majorant(density_max), density(density_fn), phase_function(make_shared<isotropic>(tex)) {}
        heterogeneous_medium(shared_ptr<hittable> boundary,double density_min, double density_max, std::function<double(const point3&)> density_fn, color& albedo)
        : boundary(boundary),d_min(density_min), majorant(density_max), density(density_fn), phase_function(make_shared<isotropic>(albedo)) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record rec1, rec2;

            if (!boundary->hit(r, interval::universe, rec1))
                return false;

            if (!boundary->hit(r, interval(rec1.t+0.0001, infinity), rec2))
                return false;

            if (rec1.t < ray_t.min) rec1.t = ray_t.min;
            if (rec2.t > ray_t.max) rec2.t = ray_t.max;

            if (rec1.t >= rec2.t)
                return false;

            if (rec1.t < 0)
                rec1.t = 0;

            auto ray_length = r.direction().length();
            auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
            
            // Woodcock Delta Tracking
            while (true) {
                auto hit_distance = (-1/majorant) * std::log(random_double());

                if (hit_distance > distance_inside_boundary) return false;

                auto t = rec1.t + hit_distance / ray_length ;
                auto p = r.at(t);
                auto sigma = density(p);

                if(sigma < d_min) sigma = d_min;
                if(sigma > majorant) sigma = majorant;

                if(random_double() < sigma / majorant){
                    // Scatter
                    rec.t = t;
                    rec.p = p;
                    rec.normal = vec3(1,0,0); //arbitrary
                    rec.front_face = true;
                    rec.mat = phase_function;
                    return true;
                }

            }
        }

        aabb bounding_box() const override { return boundary->bounding_box(); }

    private:
        shared_ptr<hittable> boundary;
        double d_min;
        double majorant;
        std::function<double(point3&)> density;
        shared_ptr<material> phase_function;
};

#endif