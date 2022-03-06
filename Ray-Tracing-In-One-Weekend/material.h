#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "rtweekend.h"

class ray;
struct hit_record;

class material
{
public:
	virtual ~material() = default;
	virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color& a) : albedo(a) {}

    bool scatter ([[maybe_unused]]const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
	const override
    {
	    auto scatter_direction = rec.normal_vec_of_hit + random_unit_vector();

        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal_vec_of_hit;
        }
        scattered = ray(rec.hit_point, scatter_direction);
        attenuation = albedo;
        return true;
    }

// ReSharper disable once CppRedundantAccessSpecifier
public:
	color albedo;
};

class metal : public material
{
public:
    metal(const color& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
	const override
    {
	    const vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_vec_of_hit);
        scattered = ray(rec.hit_point, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal_vec_of_hit) > 0);
    }

// ReSharper disable once CppRedundantAccessSpecifier
public:
    color albedo;
};

#endif