// #include "material.hpp"

// lambertian::lambertian(const color &a) : albedo(a) {}
// bool lambertian::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
// {
//     // Transfer the lambertian function we already had.
//     // Albedo is a color for the diffuse material.
//     vec3 scatter_direction = rec.normal + vec3::random_in_hemisphere(rec.normal);

//     scattered = ray(rec.p, scatter_direction);
//     attenuation = albedo;
//     return true;
// }

// metal::metal(const color &a) : albedo(a) { fuzz = 0; }
// metal::metal(const color &a, double f) : albedo(a)
// {
//     if (f < 1)
//     {
//         fuzz = f;
//     }
//     else
//     {
//         fuzz = 1;
//     }
// }

// bool metal::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
// {
//     point3 reflected = reflect(r_in.direction().normalize(), rec.normal);
//     // fuzz will multiply some randomness to alter abit of the reflection.
//     scattered = ray(rec.p, reflected + fuzz * vec3::random_in_unit_sphere());
//     attenuation = albedo;
//     // Only return if ray is not scattered into the surface. if ray scatters into surface, threat it as it got absorbed.
//     return (dot(scattered.direction(), rec.normal) > 0);
// }