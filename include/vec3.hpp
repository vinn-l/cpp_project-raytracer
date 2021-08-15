#ifndef vec3_hpp
#define vec3_hpp

#include <iostream>
#include <cmath>

// The vec3 class to hold x, y, z values and perform vector arithmetic
// point3 is also an alias to this class
class vec3
{
public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Negation
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    // Return by value
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }

    // Arithmetical operators
    vec3 &operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator-=(const vec3 &v)
    {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        e[2] -= v.e[2];
        return *this;
    }

    vec3 &operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    vec3 static random_in_unit_sphere()
    {
        while (true)
        {
            auto p = random(-1, 1);
            if (p.length() * p.length() >= 1)
                continue; // If p is not in unit sphere, try again
            return p;
        }
    }

    // This is used for lambertian distribution
    vec3 static random_in_hemisphere(const vec3 &normal)
    {
        // Pick random in unit sphere
        auto p = random_in_unit_sphere();
        // If point is in scattering hemisphere, return it else return negative of it (dot product with normal to find out)
        if (dot(p, normal) > 0) // If p same direction as normal
        {
            return p;
        }
        else
        {
            return -p;
        }
    }

private:
    vec3 static random(double min, double max)
    {
        return vec3(min + random() * (max - min), min + random() * (max - min), min + random() * (max - min));
    }

    // Random function that returns a vec3 that is randomized for diffuse/Lambertian materials
    // Will be between 0 <= r < 1
    double static random()
    {
        return rand() / double(RAND_MAX + 1.0);
    }

    friend std::ostream &operator<<(std::ostream &out, const vec3 &v);
    friend vec3 operator+(const vec3 &u, const vec3 &v);
    friend vec3 operator-(const vec3 &u, const vec3 &v);
    friend vec3 operator*(const vec3 &u, const vec3 &v);
    friend vec3 operator*(double t, const vec3 &v);
    // friend vec3 operator*(const vec3 &v, double t);
    // friend vec3 operator/(vec3 v, double t);
    friend double dot(const vec3 &u, const vec3 &v);
    friend vec3 cross(const vec3 &u, const vec3 &v);
    // friend vec3 unit_vector(vec3 v);
    double e[3];
};

// Point is alias of vec3
using point3 = vec3; // 3D point

// vec3 Utility Functions
std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

vec3 operator/(vec3 v, double t)
{
    return (1 / t) * v;
}

double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

vec3 normalize(vec3 v)
{
    return v / v.length();
}

vec3 reflect(const vec3 &v, const vec3 &n)
{
    // Reflection equation, v and n are passed as noramlized vectors
    // Dot will give projection of v on n (the magnitude), thus mutiply by n to get the vector
    return v - 2 * dot(v, n) * n;
}

#endif