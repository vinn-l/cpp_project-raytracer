Title : 
    Simple Ray Tracer with Spheres
 
Description :
    A simple ray tracer that works with only sphere shapes.
    A ray tracer is simple. One or multiple rays are projected for each pixel, and we follow the path of each ray.
    The ray will continuously scatter off objects in the world, and stops in the following cases:
        1. The ray hits a light emissive material
        2. The ray have scattered too many times (currently set to 50) and basically lost all light (return black)
        3. The ray ends up projecting into space, not hitting any object (return the background color)

    Each pixels will have (samples/pixel) amount of rays projected, and an average of each rays return value in that pixel is taken to get the color of the pixel.
    This is because there are randomness in scattering for materials (except metals with 0.0 fuzz), and thus multiple samples are used to get an average.

    Many of the math involved and the steps needed to create a ray tracing renderer is studied from these sources.
    https://raytracing.github.io/books/RayTracingInOneWeekend.html
    https://raytracing.github.io/books/RayTracingTheNextWeek.html
    Because these source's guide is pretty detailed, and that the raytracer mostly involves mathematical formulas, the code may have resulted to be pretty similar to the guide.

To run with input.txt file :
    ./tmp/install/test/raytrace < input.txt >> output.ppm

Input file :
    A .txt input file can be created to easily input parameters for setting the image properties, background, and building the world with objects.

    The file format is required to be structured as follows:
    SETTINGS samples/pixel image_width
    BACKGROUND red_top green_top blue_top red_bottom green_bottom blue_bottom
    SPHERE pos_x pos_y pos_z radius material_type mat_args...
    SPHERE …
    SPHERE …

Arguments :
    samples/pixel - Number of samples/rays projected per pixel
    image_width - Width of image (the height is automatically calculated with 16:9 ratio)
    red_top / green_top / blue_top - Color at the top for the background gradient
    red_bottom / green_bottom / blue_bottom - Color at the bottom for the background gradient
    pos_x / pos_y / pos_z - Position of the sphere. 
        - The camera is at origin (0, 0, 0), with x axis pointing to the right, y axis pointing upwards, and z axis pointing backwards of the camera's facing (Right hand rule).
    radius - Radius for the sphere
    material_type - Type of material for the sphere (more details below)
    mat_args... - Arguments for the material, such as color or fuzz (more details below)

    For creating a SPHERE, the arguments required after material_type depends on what material_type is specified
    These are the 3 material types implemented and their required arguments:
        1. LIGHT red green blue
        2. LAMBERTIAN red green blue
        3. METAL red green blue fuzz

    All colors ranges is normalized to ranges 0 to 1 instead of using 0 to 255.
    LIGHT material types can have values larger than 1 for their colors.

    Notice that metal requires an extra argument for fuzz, this represents how reflective or fuzzy the metal is ranging from 0 to 1, 0 meaning that the metal is highly reflective.
    There are a few input.txt files in the repository to demonstrate examples.

Viewport :
    The viewport is set to be 16:9 ratio, and the image is rendered to the viewport.
    (0, 0, 0) being the camera, the viewport has a height of 2, and a width of 3.56. The projection point (camera) to this plane is set to 1.
    The viewport's top left corner is at (-1.78, 1, -1), top right corner is at (1.78, 1, -1), 
    The viewport's bottom left corner is at (-1.78, -1, -1), and bottom right corner is at (1.78, -1, -1).