#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "ray.h"

class camera
{
public:
    camera()
    {
	    const double aspect_ratio = 16.0 / 9.0;
	    const double viewport_height = 2.0;
	    const double viewport_width = aspect_ratio * viewport_height;
	    const double focal_length = 1.0;

        origin = point3{ 0, 0, 0 };
        horizontal = vec3{ viewport_width, 0.0, 0.0 };
        vertical = vec3{ 0.0, viewport_height, 0.0 };
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
        // = {0,0,0} - {1,0,0} - {0, 1.77, 0} - {0,0,1} = {-1, -1.77, -1}
    }

    [[nodiscard]] ray get_ray(double u, double v) const
    {
        return ray{ origin, lower_left_corner + u * horizontal + v * vertical - origin };
        //{ (0,0,0),  (-1, -1.77, -1) + [i/width * (1,0,0)] + [j/height * (0,1.77,0)] }
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
#endif