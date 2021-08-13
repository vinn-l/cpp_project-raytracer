#ifndef sphere_hpp
#define sphere_hpp

#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include <iostream>

// Sphere equation is x^2 + y^2 + z^2 = r^2

// For a sphere located at (0,0,0)
// if x^2 + y^2 + z^2 = r^2, then x, y, z are on the sphere
// if x^2 + y^2 + z^2 > r^2, then x, y, z are outside the sphere
// if x^2 + y^2 + z^2 < r^2, then x, y, z are inside the sphere

// For a sphere located at (Cx, Cy, Cz)
// We use (x - Cx)^2 + (y - Cy)^2 + (z - Cz)^2 = r^2
// Or (P - C).(P - C) = r^2, where P is a point and C is the center of the sphere

// Therefore, if the ray P(t) = A + t*b hits the sphere, there will be some t that satisfies the above equation (P - C).(P - C) = r^2
// Look for any t where this is true
// (P(t) - C).(P(t) - C) = r^2
// (A + t*b - C).(A + t*b - C) = r^2
// (t^2)*b.b + 2tb.(A-C) + (A-C).(A-C) - r^2 = 0
// The above equation is to find the roots t

class sphere : public hittable
{
public:
    // position of sphere
    point3 center;
    // radius of sphere
    double radius;
    // radius of sphere squared
    double radius2;

    sphere(
        const vec3 &c,
        const float &r) : center(c), radius(r), radius2(r * r)
    {
    }

    // returns the value of t where the ray P(t) hits the sphere, -1 if it doesn't hit
    // note that this currently also returns true for values of t being negative (meaning sphere being behind the camera) This is a TODO to fix.
    double intersect(const ray &r) const
    {
        // b is direction of ray
        // A is the origin of the ray
        // C is the center of the sphere
        // r is radius of the sphere
        vec3 a_min_c = r.origin() - center;                        // A - C
        auto a_quad = dot(r.direction(), r.direction());           // b^2
        auto b_quad = 2 * dot(r.direction(), a_min_c);             // 2*b.(A - C)
        auto c_quad = dot(a_min_c, a_min_c) - radius2;             // (A - C).(A - C) - r^2
        auto discriminant = b_quad * b_quad - 4 * a_quad * c_quad; // b^2 - 4*a*c
        if (discriminant < 0)
            return -1;

        return (-b_quad - sqrt(discriminant)) / (2 * a_quad); // return only smallest t because that is what the camera sees
    }

    // Similar to intersect, but have t_min and t_max, and store t and normal into the hit_record instead of returning
    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
    {
        // b is direction of ray
        // A is the origin of the ray
        // C is the center of the sphere
        // r is radius of the sphere
        vec3 a_min_c = r.origin() - center;                        // A - C
        auto a_quad = dot(r.direction(), r.direction());           // b^2
        auto b_quad = 2 * dot(r.direction(), a_min_c);             // 2*b.(A - C)
        auto c_quad = dot(a_min_c, a_min_c) - radius2;             // (A - C).(A - C) - r^2
        auto discriminant = b_quad * b_quad - 4 * a_quad * c_quad; // b^2 - 4*a*c
        if (discriminant < 0)
            return false;
        else
        {
            auto root1 = (-b_quad - sqrt(discriminant)) / (2 * a_quad);
            auto root2 = (-b_quad + sqrt(discriminant)) / (2 * a_quad);
            // if roots is within t_min and t_max
            if (t_min < root1 && root1 < t_max || t_min < root2 && root2 < t_max)
            {
                // store closest root's t, p and normal to hit_record
                if (root1 < root2)
                {
                    rec.t = root1;
                }
                else
                {
                    rec.t = root2;
                }
                rec.p = r.at(rec.t);
                rec.normal = normal(rec.p); // this normal is always outwards
                rec.set_ray_side(r, rec.normal);
            }
        }
        return true;
    }

    // To get the direction of the normal of a sphere (used for reflecting rays)
    // Basically, we take the hit point of the ray and subtract with the centre of the sphere. (P - C)
    vec3 normal(const vec3 &hit_point) const
    {
        return hit_point - center;
    }
};

#endif