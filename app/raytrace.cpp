#include <iostream>
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"

// Making a simple ray tracer
// For each pixel, the ray tracer will send a ray and figure out the color met by those rays.
// 1. Shoot a ray from the camera.
// 2. Determine which objects the ray intersects.
// 3. Get the color of that intersection point.
color ray_color(const ray &r) {
    vec3 unit_direction = r.direction();

    // Create a sphere at -1 of z-axis (in front of camera), with 0.5 radius
    sphere s(vec3(0, 0, -1), 0.5);

    // Return black if the ray hits the sphere created
    auto t = s.intersect(r);
    if (s.intersect(r) > 0){ // if t is more than 0
        // Get the normal at t
        vec3 normal = s.normal(r.at(t));
        vec3 unit_normal = normal.normalize();

        // Visualize the unit normal by adding 1 and divide 2, because unit_vector range from -1 to 1
        return 0.5 * color(unit_normal.x() + 1, unit_normal.y() + 1, unit_normal.z() + 1); 
    }

    // Create a simple gradient depending on pixel position for now
    // Depending on height of ray, go from white to full red
    // unit_direction.y() goes 1 to -1, therefore add 1 to not have negative and divide by 0.5 to stay within 0 and 1
    // t now goes from 1 to 0
    t = 0.5 * (unit_direction.y() + 1.0);
    return (color(1.0, 1.0, 1.0) * t + color(0.5, 0, 0) * (1.0 - t));
}

int main() {
    // Image Properties
    const auto asp_ratio = 16 / 9;
    const int image_width = 256;
    const int image_height = image_width / asp_ratio;

    // Camera Properties
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * asp_ratio; // 3.56
    auto focal_length = 1.0; // distance between the projection plane(camera) and the projection point

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Create PPM Image
    std::cerr << "Creating PPM Image..." << std::endl;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::cerr << "\rRaytracing line " << j + 1 << " out of " << image_height << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1); // r will go from 0 to 1
            auto v = double(j) / (image_height-1); // g will go from 0 to 1
            // auto b = 0.25;

            // Ray with origin at camera, direction going towards the pixel, remember u is the pixel at horizontal (width), v is pixel at vertical (height)
            // u ranges from 0 to 1 representing width, v ranges from 0 to 1 representing height, therefore multiply with horizontal and vertical.
            // lower_left_corner represents the bottom left pixel point3.
            // So basically, lower_left_corner + u*horizontal + (1 - v)*vertical gives the pixel position, then minus origin position to get the ray direction.
            // 1 - v becase v goes from 0 to 1, but we are writing our PPM image from top to bottom, so we need to go from 1 to 0.
            ray r(origin, lower_left_corner + u*horizontal + (1 - v)*vertical - origin);

            auto pixel = ray_color(r);

            // r, g, b are in the range [0,1]
            write_color(std::cout, pixel);
        }
    }

    std::cerr << "\nPPM Image Created" << std::endl;
    return 0;
}