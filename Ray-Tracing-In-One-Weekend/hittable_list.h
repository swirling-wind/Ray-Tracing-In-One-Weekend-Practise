#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { hit_objects.clear(); }
    void add(shared_ptr<hittable> object) { hit_objects.push_back(object); }

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec)
        const override;

public:
    std::vector<shared_ptr<hittable>> hit_objects;
};

inline bool hittable_list::hit(const ray& r, const double t_min, const double t_max,
    hit_record& rec)
    const
{
    hit_record temp_record;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : hit_objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_record))
        {
            hit_anything = true;
            closest_so_far = temp_record.t_of_ray;
            rec = temp_record;
        }
    }

    return hit_anything;
}

#endif