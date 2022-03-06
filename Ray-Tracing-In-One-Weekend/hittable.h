#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record
{
    point3 hit_point;
    vec3 normal_vec_of_hit;
    double t_of_ray{ 0.0 };//
    bool front_face{ false };//

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal_vec_of_hit = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual ~hittable() = default;//
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif