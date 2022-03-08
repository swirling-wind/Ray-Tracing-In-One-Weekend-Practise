#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "ray.h"

class camera
{
public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double vertical_field_of_view, double aspect_ratio)
    {
	    const double theta = degrees_to_radians(vertical_field_of_view);
	    const double h = tan(theta / 2);
	    const double viewport_height = 2.0 * h;
	    const double viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;

        // PREVIOUS FIXED VERSION
		//  constexpr double focal_length = 1.0;
		//   origin = point3{ 0, 0, 0 };
		//   horizontal = vec3{ viewport_width, 0.0, 0.0 };
		//   vertical = vec3{ 0.0, viewport_height, 0.0 };
		//   lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
        //   = {0,0,0} - {1,0,0} - {0, 1.77, 0} - {0,0,1} = {-1, -1.77, -1}
    }

    //[[nodiscard]] ray fixed_get_ray(double u, double v) const
    //{
    //    return ray{ origin, lower_left_corner + u * horizontal + v * vertical - origin };
    //    //{ (0,0,0),  (-1, -1.77, -1) + [i/width * (1,0,0)] + [j/height * (0,1.77,0)] }
    //}

    [[nodiscard]] ray get_ray(double s, double t) const
    {
        return ray{ origin, lower_left_corner + s * horizontal + t * vertical - origin };
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
#endif