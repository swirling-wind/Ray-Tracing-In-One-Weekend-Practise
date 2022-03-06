#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere final : public hittable
{
public:
	sphere() = default;
	sphere(point3 cen, double r) : center{ cen }, radius{ r } {}

	bool hit(
		const ray& r, double min_t_of_ray, double max_t_of_ray, hit_record& record)
		const override;

public:
	point3 center;
	double radius;
};

inline bool sphere::hit(const ray& r, double min_t_of_ray, double max_t_of_ray, hit_record& record) const
{
	vec3 oc = r.origin() - center;
	auto half_b = dot(oc, r.direction());
	auto a = r.direction().length_squared();
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return false;
	const auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < min_t_of_ray || max_t_of_ray < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < min_t_of_ray || max_t_of_ray < root)
		{
			return false;
		}
	}

	record.t_of_ray = root;
	record.hit_point = r.at(record.t_of_ray);

	vec3 outward_normal = (record.hit_point - center) / radius;
	record.set_face_normal(r, outward_normal);

	return true;
}

#endif