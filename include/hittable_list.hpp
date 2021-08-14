#ifndef hittable_list_hpp
#define hittable_list_hpp

#include "material.hpp"
#include <vector>

class hittable_list
{
public:
    // Constructor
    hittable_list(){};

    void add(hittable* object){
        hittables.push_back(object);
    }

    void clear()
    {
        hittables.clear();
    }

    // function that takes in a ray to see if the ray hits what hittables in the list
    bool hit_all(const ray &ray_in, double t_min, double t_max, hit_record &rec) const
    {
        hit_record curr_record;
        bool hit_something = false;
        auto t = t_max;

        // Loop each hittables in list
        for (auto i = 0; i < hittables.size(); i++)
        {
            // If the ray hits the hittable, and that t is smaller than the t currently, means the object is nearer to camera, thus store into curr_record
            // Arrow because hittables[i] is a pointer hittable *
            if (hittables[i]->hit(ray_in, t_min, t, curr_record))
            {
                hit_something = true;

                // set t to the smallest t value
                t = curr_record.t;
                rec = curr_record;
            }
        }
        return hit_something;
    }

private:
    // list of hittable
    std::vector<hittable*> hittables;
};

#endif