#ifndef color_hpp
#define color_hpp

#include "color.hpp"
#include <iostream>

using std::sqrt;

// The color class, similar to color but seperated into individual classes for error checking.
class color
{
public:
    color() : e{0, 0, 0} {}
    color(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double r() const { return e[0]; }
    double g() const { return e[1]; }
    double b() const { return e[2]; }

private:
    friend std::ostream &operator<<(std::ostream &out, const color &v);
    friend color operator+(const color &u, const color &v);
    friend color operator-(const color &u, const color &v);
    friend color operator*(const color &u, const color &v);
    friend color operator*(double t, const color &v);
    // friend color operator*(const color &v, double t);
    // friend color operator/(color v, double t);
    friend double dot(const color &u, const color &v);
    friend color cross(const color &u, const color &v);
    // friend color unit_vector(color v);
    double e[3];
};

// Function to easily write one color color pixel to output stream
// Arguments are output stream and color color
void write_color(std::ostream &os, const color &color)
{
    os << static_cast<int>(256 * color.r()) << " " << static_cast<int>(256 * color.g()) << " " << static_cast<int>(256 * color.b()) << "\n";
}

// color Utility Functions
std::ostream &operator<<(std::ostream &out, const color &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

color operator+(const color &u, const color &v)
{
    return color(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

color operator-(const color &u, const color &v)
{
    return color(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

color operator*(const color &u, const color &v)
{
    return color(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

color operator*(double t, const color &v)
{
    return color(t * v.e[0], t * v.e[1], t * v.e[2]);
}

color operator*(const color &v, double t)
{
    return t * v;
}

color operator/(color v, double t)
{
    return (1 / t) * v;
}

double dot(const color &u, const color &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

color cross(const color &u, const color &v)
{
    return color(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

#endif