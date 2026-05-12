#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class quad : public hittable {
    public:
        quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) 
        :Q(Q), u(u), v(v), mat(mat){
            auto n = cross(u,v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n,n);
            set_bounding_box();
        }

        virtual void set_bounding_box() {
            // Compute the bounding box of all four vertices
            auto bbox_diagonal1 = aabb(Q,Q+u+v);
            auto bbox_diagonal2 = aabb(Q+u,Q+v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            auto denom = dot(normal, r.direction());

            // No hit if the ray is parallel to the plane
            if(std::fabs(denom) < 1e-8) return false;

            // Return false if the hit is outside the ray interval
            auto t = (D - dot(normal, r.origin()))/denom;

            if(!ray_t.contains(t)) return false;

            // Determine whether the intersection point lies within the quad
            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w,cross(u,planar_hitpt_vector));
            
            if(!is_interior(alpha, beta, rec)){
                return false;
            }

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r,normal);
            return true;
        }

        virtual bool is_interior(double a, double b, hit_record& rec) const {
            interval unit_interval = interval(0,1);

            if(!unit_interval.contains(a) || !unit_interval.contains(b)){
                return false;
            }

            rec.u = a;
            rec.v = b;
            return true;
        }

    protected:
        point3 Q;
        vec3 u;
        vec3 v;
        vec3 w;
        shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;
        double D;
};

// Implementing a disc class on my own :)
class disc : public quad {
    public:
        disc(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) : quad(Q,u,v,mat) {
            set_bounding_box();
        }

        virtual void set_bounding_box() override {
            // Computer the bounding box
            aabb bbox_diameter1 = aabb(Q+u, Q-u);
            aabb bbox_diameter2 = aabb(Q+v, Q-v);
            bbox = aabb(bbox_diameter1, bbox_diameter2);
        }

        virtual bool is_interior(double a, double b, hit_record& rec) const override {

            // Fix the coordinates to lie from -1 to 1
            double x = 2*a - 1;
            double y = 2*b - 1;

            if(x*x + y*y > 1){
                return false;
            }

            rec.u = a;
            rec.v = b;

            return true;
        }
};

class annular_disc : public quad {
    public:
        annular_disc(const point3& Q, const vec3& u, const vec3& v, double inner_radius, shared_ptr<material> mat) : quad(Q,u,v,mat), inner_radius(std::fmin(inner_radius,u.length())/u.length()) {
            set_bounding_box();
        }

        virtual void set_bounding_box() override {
            // Computer the bounding box
            bbox = aabb(Q-u-v, Q+u+v);
        }

        virtual bool is_interior(double a, double b, hit_record& rec) const override {

            // Fix the coordinates to lie from -1 to 1
            double x = 2*a - 1;
            double y = 2*b - 1;

            double r2 = x*x + y*y;
            if(r2 > 1){
                return false;
            }

            if(r2 < inner_radius*inner_radius){
                return false;
            }

            rec.u = a;
            rec.v = b;

            return true;
        }
    
    private:
        double inner_radius;
        vec3 delta_u, delta_v;
};

#endif