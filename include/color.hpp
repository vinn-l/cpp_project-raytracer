#ifndef color_hpp
#define color_hpp

#include "color.hpp"
#include <iostream>

using std::sqrt;

// The color class, similar to vec3 but seperated into individual classes for error checking.
class color {
    public:
        color() : e{0,0,0} {}
        color(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double r() const { return e[0]; }
        double g() const { return e[1]; }
        double b() const { return e[2]; }

    private:
        double e[3];
};

// Function to easily write one color color pixel to output stream
// Arguments are output stream and color color
void write_color(std::ostream& os, const color& color){
    os << static_cast<int>(256 * color.r()) << " " << static_cast<int>(256 * color.g()) << " " << static_cast<int>(256 * color.b()) << "\n";
}

#endif