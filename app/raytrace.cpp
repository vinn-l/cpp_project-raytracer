#include <iostream>
#include "vec3.hpp"
#include "color.hpp"

int main() {

    // Image Properties

    const int image_width = 256;
    const int image_height = 256;

    // Create PPM Image
    std::cerr << "Creating PPM Image..." << std::endl;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::cerr << "\rRaytracing line " << j + 1 << " out of " << image_height << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width-1); // r will go from 0 to 1
            auto g = double(j) / (image_height-1); // g will go from 0 to 1
            auto b = 0.25;

            auto pixel = color(r, g, b);

            // r, g, b are in the range [0,1]
            write_color(std::cout, pixel);
        }
    }

    std::cerr << "\nPPM Image Created" << std::endl;
    return 0;
}