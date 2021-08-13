#ifndef vec3_hpp
#define vec3_hpp

#include <cmath>
#include <iostream>

using std::sqrt;

// The vec3 class
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

    vec3 &operator*=(const double t)
    {
        return *this *= t;
    }

    vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    vec3 &normalize()
    {
        double l = length();
        e[0] /= l;
        e[1] /= l;
        e[2] /= l;
        return *this;
    }

private:
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

vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

vec3 normalize(vec3 v)
{
    return v / v.length();
}

#endif