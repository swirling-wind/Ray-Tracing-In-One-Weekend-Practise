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
	explicit lambertian(const color& a) : albedo(a) {}

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
    explicit metal(const color& new_albedo, double fuzziness) : albedo(new_albedo), fuzz(fuzziness < 1 ? fuzziness : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
	const override
    {
	    const vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_vec_of_hit);
        scattered = ray(rec.hit_point, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal_vec_of_hit) > 0);
    }

// ReSharper disable once CppRedundantAccessSpecifier
public:
    color albedo;
    double fuzz;
};

class dielectric : public material
{
public:
	explicit dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
	const override
    {
        const double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
        const vec3 unit_direction = unit_vector(r_in.direction());

        const double cos_theta = fmin(dot(-unit_direction, rec.normal_vec_of_hit), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        vec3 direction;
        if (refraction_ratio * sin_theta > 1.0)
            direction = reflect(unit_direction, rec.normal_vec_of_hit);
        else
            direction = refract(unit_direction, rec.normal_vec_of_hit, refraction_ratio);
        
        scattered = ray(rec.hit_point, direction);
        attenuation = color(1.0, 1.0, 1.0); //glass surface absorbs nothing
        return true;
    }

// ReSharper disable once CppRedundantAccessSpecifier
public:
    double ir; // Index of Refraction
};

#endif