#ifndef sphere_hpp
#define sphere_hpp

#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
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

class sphere
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

    // returns true of the ray intersects with the sphere
    bool intersect(const ray &r) const
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

        return true;
    }
};

#endif