// A ray's function is P(t) = A + tb, where P is the ray's position, A is the ray's origin, and b is the ray's direction. t stands for time.

#include "vec3.hpp"

#ifndef RAY_H
#define RAY_H

class ray
{
public:
    ray() {}
    ray(const point3 &p, const vec3 &d) : orig(p), dir(d) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    // Following P(t) = A + tb
    point3 at(double t) const { return orig + t * dir; }

private:
    // Ray has a point and a direction
    point3 orig;
    vec3 dir;
};

#endif