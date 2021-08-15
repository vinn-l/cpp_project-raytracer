#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <sstream>

#define MAX_DEPTH 50
// #define SAMPLES_PER_PIXEL 50 // Increase this to get better quality, but requires more time

// A simple ray tracer
// For each pixel, the ray tracer will send (samples_p_pixel) number of rays and figure out the color met by those rays.
// 1. Shoot multiple ray from the camera. (Rays are slightly altered directions but still towards that pixel)
// 2. Determine which objects the ray intersects.
// 3. Stops when it either hits a light source, it reflected too many times, or it was absorbed by metal object.
color ray_color(const ray &r, const hittable_list &world, int depth, const color background_color_top, const color background_color_bottom)
{
    hit_record rec;

    // If we reflected way to many times, light is all absorbed.
    if (depth > MAX_DEPTH)
    {
        return color(0, 0, 0);
    }

    // Find the closest hittable and render that hittable's color
    // hit(const ray &ray_in, double t_min, double t_max, hit_record &rec)
    // we set t_min as 0.001 because sometimes the root is calculated to be very small value that is just intersecting with the object that the ray just reflected off, so we want to ignore these cases.
    if (world.hit_all(r, (double)0.001, std::numeric_limits<double>::infinity(), rec))
    {
        ray reflected_ray;
        color attenuation;
        color emitted = rec.mat->emitted();
        // If ray is reflected
        if (rec.mat->scatter(r, rec, attenuation, reflected_ray))
        {
            // Attenuation is the color of the material, and will cause bias to color (alter the color of further objects being hit by ray)
            return attenuation * ray_color(reflected_ray, world, depth + 1, background_color_top, background_color_bottom);
        }

        // If no scatter, means ray either hit a light or ray is absorbed by metal
        return emitted;
    }

    // If ray hits nothing, we return background color

    // Create a simple gradient depending on pixel position
    // Depending on height of ray, go from white to full red
    // unit_direction.y() goes 1 to -1, therefore add 1 to not have negative and divide by 0.5 to stay within 0 and 1
    // t now goes from 1 to 0
    vec3 unit_direction = r.direction();
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (background_color_bottom * (1 - t) + background_color_top * (t));
}

int main()
{
    // Input file parser
    std::vector<std::vector<std::string>> lines;
    std::string line;

    int line_number = 0;
    while (std::getline(std::cin, line))
    {
        std::vector<std::string> args;
        std::string word;
        std::istringstream iss(line);
        while (iss >> word)
        {
            args.push_back(word);
        }
        lines.push_back(args);
    }

    const int samples_p_pixel = std::stoi(lines[0][1]);
    const int image_width = std::stoi(lines[0][2]);

    const color background_colour_top = color(std::stod(lines[1][1]), std::stod(lines[1][2]), std::stod(lines[1][3]));
    const color background_colour_bottom = color(std::stod(lines[1][4]), std::stod(lines[1][5]), std::stod(lines[1][6]));
    hittable_list world;

    // Number of objects
    size_t num_objects = lines.size() - 2;
    std::vector<material> material_objs;
    std::vector<hittable> hittable_objs;
    for (int i = 0; i < num_objects; i++)
    {
        // if line is empty, skip
        if (lines[i + 2].empty())
        {
            continue;
        }
        std::vector<std::string> args = lines[i + 2];
        if (args[5] == "LAMBERTIAN")
        {
            // New required here because its in a for loop and will get overriden if dynamic allocation is not done.
            material *material_obj = new lambertian(color(std::stod(args[6]), std::stod(args[7]), std::stod(args[8])));
            sphere *sphere_obj = new sphere(vec3(std::stod(args[1]), std::stod(args[2]), std::stod(args[3])), std::stod(args[4]), material_obj);
            world.add(sphere_obj);
        }
        else if (args[5] == "LIGHT")
        {
            // New required here because its in a for loop and will get overriden if dynamic allocation is not done.
            material *material_obj = new diffuse_light(color(std::stod(args[6]), std::stod(args[7]), std::stod(args[8])));
            sphere *sphere_obj = new sphere(vec3(std::stod(args[1]), std::stod(args[2]), std::stod(args[3])), std::stod(args[4]), material_obj);
            world.add(sphere_obj);
        }
        else if (args[5] == "METAL")
        {
            // New required here because its in a for loop and will get overriden if dynamic allocation is not done.
            material *material_obj = new metal(color(std::stod(args[6]), std::stod(args[7]), std::stod(args[8])), std::stod(args[9]));
            sphere *sphere_obj = new sphere(vec3(std::stod(args[1]), std::stod(args[2]), std::stod(args[3])), std::stod(args[4]), material_obj);
            world.add(sphere_obj);
        }
        else
        {
            std::cerr << "Error: Invalid material type " << args[5] << std::endl;
            return 1;
        }
    }

    // Image Properties
    const double asp_ratio = 16.0 / 9.0;
    const int image_height = (int)(image_width / asp_ratio);

    // Camera Properties
    const double viewport_height = 2.0;
    const double viewport_width = viewport_height * asp_ratio; // 3.56
    // Distance between the projection plane(camera) and the projection point
    // Smaller means more zoomed in
    // Larger means more zoomed out
    const double focal_length = 1.0;

    const point3 origin = point3(0, 0, 0);
    const vec3 horizontal = vec3(viewport_width, 0, 0);
    const vec3 vertical = vec3(0, viewport_height, 0);
    const point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Create PPM Image
    std::cerr << "Creating PPM Image..." << std::endl;

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        std::cerr << "\rRaytracing horizontal line " << j + 1 << " out of " << image_height << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);

            // Shoot multiple samples for anti-aliasing
            for (int sample = 0; sample < samples_p_pixel; sample++)
            {
                // Add a random value between 0 and 1 for multiple samples
                auto u = (double(i) + rand() / (RAND_MAX + 1.0)) / (image_width - 1);  // u will go from 0 to 1
                auto v = (double(j) + rand() / (RAND_MAX + 1.0)) / (image_height - 1); // v will go from 0 to 1

                // Ray with origin at camera, direction going towards the pixel, remember u is the pixel at horizontal (width), v is pixel at vertical (height)
                // u ranges from 0 to 1 representing width, v ranges from 0 to 1 representing height, therefore multiply with horizontal and vertical.
                // lower_left_corner represents the bottom left pixel point3.
                // So basically, lower_left_corner + u*horizontal + (1 - v)*vertical gives the pixel position (viewport coordinates), then minus origin position to get the ray direction.
                // 1 - v becase v goes from 0 to 1, but we are writing our PPM image from top to bottom, so we need to go from 1 to 0.
                ray r(origin, lower_left_corner + u * horizontal + (1 - v) * vertical - origin);

                // Summation of the samples
                pixel_color += ray_color(r, world, 0, background_colour_top, background_colour_bottom);
            }
            // Get average of the samples for each pixel
            pixel_color /= samples_p_pixel;

            // Gamma-correct for gamma=2.0.
            pixel_color = color(std::sqrt(pixel_color.r()), std::sqrt(pixel_color.g()), std::sqrt(pixel_color.b()));

            // r, g, b are in the range [0,1]
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nPPM Image Created" << std::endl;
    return 0;
}