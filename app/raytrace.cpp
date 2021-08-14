#include <iostream>
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include <limits>
#include <string>
#include <iostream>
#include <sstream>

#define MAX_DEPTH 50
#define SAMPLES_PER_PIXEL 50 // Increase this to get better quality, but requires more time

// Making a simple ray tracer
// For each pixel, the ray tracer will send a ray and figure out the color met by those rays.
// 1. Shoot a ray from the camera.
// 2. Determine which objects the ray intersects.
// 3. Get the color of that intersection point.
color ray_color(const ray &r, const hittable_list &world, int depth, color *background_color_top, color *background_color_bottom)
{
    hit_record rec;

    // If we reflected way to many times, light is all absorbed.
    if (depth > MAX_DEPTH)
    {
        return color(0, 0, 0);
    }

    // find the closest hittable and render that hittable's color
    // hit(const ray &ray_in, double t_min, double t_max, hit_record &rec)
    // we set t_min as 0.001 because sometimes the root is calculated to be very small value that is just intersecting with the object that the ray just reflected off, so we want to ignore these cases.
    if (world.hit_all(r, (double)0.001, std::numeric_limits<double>::infinity(), rec))
    {
        ray reflected_ray;
        color attenuation;
        color emitted = rec.mat->emitted();
        // if ray is reflected
        if (rec.mat->scatter(r, rec, attenuation, reflected_ray))
        {
            // attenuation is the color of the material, and will cause bias to color (alter the color of further objects being hit by ray)
            return emitted + attenuation * ray_color(reflected_ray, world, depth + 1, background_color_top, background_color_bottom);
        }
        // else ray hit a light, return its light emission color
        return emitted;

        // find the reflection ray
        // point3 random_reflect_target = rec.p + vec3::random_in_hemisphere(rec.normal);

        // ray trace the reflection ray
        // 0.5 here is to reduce the light intensity by 0.5 everytime it is reflected
        // return 0.5 * ray_color(ray(rec.p, random_reflect_target - rec.p), world, depth + 1);
    }

    // If ray hits nothing, we return background (blue sky gradient)

    // Create a simple gradient depending on pixel position
    // Depending on height of ray, go from white to full red
    // unit_direction.y() goes 1 to -1, therefore add 1 to not have negative and divide by 0.5 to stay within 0 and 1
    // t now goes from 1 to 0
    vec3 unit_direction = r.direction();
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (*background_color_bottom * (1 - t) + *background_color_top * (t));

    // Black background
    // return color(0,0,0);
}

int main()
{
    // Take input from stdin
    // const char config[] = "url=http://example.com\n"
    //                       "file=main.exe\n"
    //                       "true=0";

    // std::istringstream is_file(config);

    std::vector<std::vector<std::string>> lines;
    std::string line;
    std::string settings[3];
    std::string background[4];
    std::string sphere_arr[10]; // If metal 10, if light or diffuse, will have 9

    int line_number = 0;
    while (std::getline(std::cin, line))
    {
        std::vector<std::string> args;
        std::string word;
        std::istringstream iss(line);
        while (iss >> word)
        {
            args.push_back(word);
            // std::cerr << word;
        }

        // std::cerr << std::endl;
        // store into vector
        lines.push_back(args);

        // // If line_number = 0, means settings
        // if (line_number == 0)
        // {
        //     while (std::cin >> word)
        //     {
        //         samples_p_pixel = word;
        //         i++;
        //     }
        // }

        // // Parse line by line
        // int i = 0;

        // std::string word;
        // while (std::cin >> word)
        // {
        //     std::cerr << word << std::endl;
        // }
        // line_number++;
    }

    auto samples_p_pixel = std::stoi(lines[0][1]);
    auto image_width = std::stoi(lines[0][2]);

    auto background_colour_top = color(std::stod(lines[1][1]), std::stod(lines[1][2]), std::stod(lines[1][3]));
    auto background_colour_bottom = color(std::stod(lines[1][4]), std::stod(lines[1][5]), std::stod(lines[1][6]));
    hittable_list world;
    // number of objects
    size_t num_spheres = lines.size() - 2;
    for (int i = 0; i < num_spheres; i++)
    {
        std::vector<std::string> args = lines[i + 2];
        if (args[5] == "lambertian")
        {
            std::cerr << "lambertian" << std::endl;
            std::cerr << args[1] << " " << args[2] << " " << args[3] << " " << args[4] << " " << args[5] << " " << args[6] << " " << args[7] << " " << args[8] << std::endl;
            world.add(&sphere(vec3(std::stod(args[1]), std::stod(args[2]), std::stod(args[3])), std::stod(args[4]), &lambertian(color(std::stod(args[6]), std::stod(args[7]), std::stod(args[8])))));
        }
        else if (args[5] == "light")
        {
            std::cerr << "light" << std::endl;
            world.add(&sphere(vec3(std::stod(args[1]), std::stod(args[2]), std::stod(args[3])), std::stod(args[4]), &diffuse_light(color(std::stod(args[6]), std::stod(args[7]), std::stod(args[8])))));
        }
        else if (args[5] == "metal")
        {
            std::cerr << "metal" << std::endl;
            world.add(&sphere(vec3(std::stod(args[1]), std::stod(args[2]), std::stod(args[3])), std::stod(args[4]), &metal(color(std::stod(args[6]), std::stod(args[7]), std::stod(args[8])), std::stod(args[9]))));
        }
        else{
            std::cerr << "Error: Invalid material type" << std::endl;
            return 1;
        }
    }

    // Image Properties
    const auto asp_ratio = 16.0 / 9.0;
    // const int image_width = 500;
    const int image_height = (int)(image_width / asp_ratio);

    // Some default colors
    color red(1.0, 0.0, 0.0);
    color yellow(1.0, 1.0, 0.0);
    color green(0.0, 1.0, 0.0);
    color cyan(0.0, 1.0, 1.0);
    color blue(0.0, 0.0, 1.0);
    color purple(1.0, 0.0, 1.0);
    color white(1.0, 1.0, 1.0);
    color black(0.0, 0.0, 0.0);

    // World
    // hittable_list world;
    lambertian lambertian1(color(0.8, 0.8, 0.0));
    lambertian lambertian2(color(0.7, 0.3, 0.3));
    lambertian lambertian3(color(0.4, 0.2, 0.8));
    lambertian lambertian4(color(0.4, 0.2, 0.8));
    lambertian lambertian5(color(1.0, 0.0, 1.0));
    lambertian lambertian6(color(0.1, 0.1, 0.8));
    // lambertian lambertian_red(red);
    // lambertian lambertian_green(green);
    // lambertian lambertian_cyan(cyan);
    // lambertian lambertian_white(white);
    metal metal1(color(0.8, 0.8, 0.8), 0.7);
    dielectric dielectric1(1.5);
    dielectric dielectric2(1.2);
    metal metal2(color(0.0, 0.5, 0.5), 1.0);
    diffuse_light light1(color(4.0, 4.0, 4.0));
    sphere sphere1(vec3(-0.75, 0, -1), 0.5, &lambertian1);
    sphere sphere2(vec3(0.25, 0.5, -0.5), 0.15, &metal2);
    sphere sphere3(vec3(0.62, 0.5, -1.0), 0.45, &lambertian3);
    sphere sphere4(vec3(-1.0, 0.75, -0.5), 0.5, &light1);
    sphere sphere5(vec3(0, 0, -1), 0.5, &metal1);
    sphere sphere6(vec3(0, -100.5, -1), 100, &lambertian2);
    // world.add(&sphere1);
    // world.add(&sphere2);
    // world.add(&sphere3);
    // world.add(&sphere4);
    // world.add(&sphere5);
    // world.add(&sphere6);

    // Camera Properties
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * asp_ratio; // 3.56
    auto focal_length = 1.0;                           // distance between the projection plane(camera) and the projection point

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

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
                // add a random value between 0 and 1 but not 1
                auto u = (double(i) + rand() / (RAND_MAX + 1.0)) / (image_width - 1);  // u will go from 0 to 1
                auto v = (double(j) + rand() / (RAND_MAX + 1.0)) / (image_height - 1); // v will go from 0 to 1

                ray r(origin, lower_left_corner + u * horizontal + (1 - v) * vertical - origin);

                // summation of the pixel colors
                pixel_color += ray_color(r, world, 0, &background_colour_top, &background_colour_bottom);
            }
            // Get average of the samples for each pixel
            pixel_color /= samples_p_pixel;

            // Gamma-correct for gamma=2.0.
            pixel_color = color(sqrt(pixel_color.r()), sqrt(pixel_color.g()), sqrt(pixel_color.b()));

            // auto b = 0.25;

            // Ray with origin at camera, direction going towards the pixel, remember u is the pixel at horizontal (width), v is pixel at vertical (height)
            // u ranges from 0 to 1 representing width, v ranges from 0 to 1 representing height, therefore multiply with horizontal and vertical.
            // lower_left_corner represents the bottom left pixel point3.
            // So basically, lower_left_corner + u*horizontal + (1 - v)*vertical gives the pixel position, then minus origin position to get the ray direction.
            // 1 - v becase v goes from 0 to 1, but we are writing our PPM image from top to bottom, so we need to go from 1 to 0.
            // ray r(origin, lower_left_corner + u * horizontal + (1 - v) * vertical - origin);

            // auto pixel = ray_color(r, world, 0);

            // r, g, b are in the range [0,1]
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nPPM Image Created" << std::endl;
    return 0;
}