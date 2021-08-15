// Materials and their functions/maths are studied from https://raytracing.github.io/books/RayTracingInOneWeekend.html
// and light material from https://raytracing.github.io/books/RayTracingTheNextWeek.html

#ifndef material_hpp
#define material_hpp

#include "ray.hpp"
#include "color.hpp"
#include "vec3.hpp"
#include "sphere.hpp"
#include "color.hpp"

class material;

// A record of a hit, to hold the point of the hit, the normal(for sphere) and the t value and the material of the object being hit.
struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    material *mat;
};

class hittable
{
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

// Material abstract class to contain the abstract method hit for each different material to implement
class material
{
public:
    virtual color emitted() const
    {
        return color(0, 0, 0);
    }
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
            // Maximum fuzz is 1
            fuzz = 1;
        }
    }

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        vec3 scatter_direction = reflect(normalize(r_in.direction()), rec.normal);

        // Fuzz will add some randomness to alter abit of the reflection.
        scattered = ray(rec.p, scatter_direction + fuzz * vec3::random_in_unit_sphere());
        attenuation = albedo;
        
        // Only return if ray is not scattered into the surface. if ray scatters into surface, threat it as it got absorbed.
        // It is only possible for ray to scatter into surface if the fuzz made it so.
        return (dot(scattered.direction(), rec.normal) >= 0);
    }

    color albedo;
    double fuzz;
};

class diffuse_light : public material
{
public:
    diffuse_light(color c) : emit(c) {}

    bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        // Light does not scatter but immediately returns the light
        return false;
    }

    color emitted() const
    {
        return emit;
    }

    color emit;
};

#endif