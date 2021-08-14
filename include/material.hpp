#ifndef material_hpp
#define material_hpp

#include "ray.hpp"
#include "color.hpp"
#include "vec3.hpp"
#include "sphere.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include <vector>

class material;

// a record of a hit, to hold the point of the hit, the normal(for sphere) and the t value for the hit
struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    material *mat;

    // This bool will tell whether a ray is coming from the inside or the outside of the object (for sphere)
    bool ray_inside;

    // This function will help determine whether the ray is coming from inside or outside 
    // Recall the normal() function in sphere class always takes P - C thus always points outwards in direction C to P
    // The normal given to this function will always be pointing outwards
    void set_ray_side(const ray &r, const vec3 &outward_normal)
    {
        // If dot of ray and normal is less than 0, it means they both are in the same direction
        ray_inside = dot(r.direction(), outward_normal) < 0;

        // Set the normal for the record to point towards inside if ray is coming from inside
        normal = ray_inside ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

class material
{
public:
    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color &a) : albedo(a) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        // Transfer the lambertian function we already had.
        // Albedo is a color for the diffuse material.
        vec3 scatter_direction = rec.normal + vec3::random_in_hemisphere(rec.normal);

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
    color albedo;
};

class metal : public material
{
public:
    metal(const color &a) : albedo(a) { fuzz = 0; }
    metal(const color &a, double f) : albedo(a)
    {
        if (f < 1)
        {
            fuzz = f;
        }
        else
        {
            fuzz = 1;
        }
    }

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        point3 reflected = reflect(r_in.direction().normalize(), rec.normal);
        // fuzz will multiply some randomness to alter abit of the reflection.
        scattered = ray(rec.p, reflected + fuzz * vec3::random_in_unit_sphere());
        attenuation = albedo;
        // Only return if ray is not scattered into the surface. if ray scatters into surface, threat it as it got absorbed.
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    color albedo;
    double fuzz;
};
#endif