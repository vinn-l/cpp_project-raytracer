#include <iostream>
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "hittable.hpp"
#include <limits>

#define MAX_DEPTH 50

// Making a simple ray tracer
// For each pixel, the ray tracer will send a ray and figure out the color met by those rays.
// 1. Shoot a ray from the camera.
// 2. Determine which objects the ray intersects.
// 3. Get the color of that intersection point.
color ray_color(const ray &r, const hittable_list& world, int depth) {
    hit_record rec;

    // If we reflected way to many times, light is all absorbed.
    if (depth > MAX_DEPTH) {
        return color(0, 0, 0);
    }

    // find the closest hittable and render that hittable's color
    // hit(const ray &ray_in, double t_min, double t_max, hit_record &rec)
    if(world.hit_all(r, (double)0, std::numeric_limits<double>::infinity(), rec)){
        // return 0.5 * color(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);

        // find the reflection ray
        point3 random_reflect_target = rec.p + rec.normal + vec3::random_in_unit_sphere();

        // ray trace the reflection ray
        return 0.5 * ray_color(ray(rec.p, random_reflect_target - rec.p), world, depth + 1);
    }

    // Create a simple gradient depending on pixel position for now
    // Depending on height of ray, go from white to full red
    // unit_direction.y() goes 1 to -1, therefore add 1 to not have negative and divide by 0.5 to stay within 0 and 1
    // t now goes from 1 to 0
    vec3 unit_direction = r.direction();
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (color(1.0, 1.0, 1.0) * t + color(0.5, 0, 0) * (1.0 - t));
}

int main() {
    // Image Properties
    const auto asp_ratio = 16.0 / 9.0;
    const int image_width = 500;
    const int image_height = (int)(image_width / asp_ratio);

    // World
    hittable_list world;
    sphere sphere1(vec3(-0.75, -0.5, -1), 0.5);
    sphere sphere2(vec3(0.25, 0.5, -1.5), 0.5);
    sphere sphere3(vec3(0.0, 0.0, -30.0), 10.0);
    sphere sphere4(vec3(-1.0, 1.0, -0.5), 0.5);
    sphere sphere5(vec3(0,0,-1), 0.5);
    sphere sphere6(vec3(0,-100.5,-1), 100);
    world.add(&sphere1);
    world.add(&sphere2);
    world.add(&sphere3);
    world.add(&sphere4);
    world.add(&sphere5);
    world.add(&sphere6);

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

            auto pixel = ray_color(r, world, 0);

            // r, g, b are in the range [0,1]
            write_color(std::cout, pixel);
        }
    }

    std::cerr << "\nPPM Image Created" << std::endl;
    return 0;
}