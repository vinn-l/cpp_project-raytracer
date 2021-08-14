// #ifndef hittable_hpp
// #define hittable_hpp

// #include "ray.hpp"
// #include "material.hpp"

// // a record of a hit, to hold the point of the hit, the normal(for sphere) and the t value for the hit
// struct hit_record
// {
//     point3 p;
//     vec3 normal;
//     double t;
//     material *mat;

//     // This bool will tell whether a ray is coming from the inside or the outside of the object (for sphere)
//     bool ray_inside;

//     // This function will help determine whether the ray is coming from inside or outside 
//     // Recall the normal() function in sphere class always takes P - C thus always points outwards in direction C to P
//     // The normal given to this function will always be pointing outwards
//     void set_ray_side(const ray &r, const vec3 &outward_normal)
//     {
//         // If dot of ray and normal is less than 0, it means they both are in the same direction
//         ray_inside = dot(r.direction(), outward_normal) < 0;

//         // Set the normal for the record to point towards inside if ray is coming from inside
//         normal = ray_inside ? outward_normal : -outward_normal;
//     }
// };

// class hittable
// {
// public:
//     virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
// };

// #endif