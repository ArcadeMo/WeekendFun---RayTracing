#include "utils.h"

#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"

/* Function to determine if a given ray hits a sphere; returns true if the ray intersects the sphere
    // bool hitSphere(const point3& center, double radius, const ray& r) {
    //     // oc is the vector from the ray's origin to the spheres center 
    //     vec3 oc = center - r.origin();
    //     // a represents the squared length of the ray's direction vector using the dot product
    //     auto a = dot(r.direction(), r.direction());
    //     // b represents twice the negative dot product of the ray's direction and the vector from ray's origin to center of the sphere
    //     auto b = -2.0 * dot(r.direction(), oc);
    //     // c calculates the squared distnace from the ray's origin to the sphere minus the square of the radius
    //     auto c = dot(oc, oc) - radius*radius;
    //     // The discriminant of the quadratic equation determines where the ray intersects the sphere; if it is greater than or equal to 0 the ray intersects the sphere
    //     auto discriminant = b*b - 4*a*c;
    //     return (discriminant >= 0);
    // }

// Old Function to determine if a given ray hits a sphere; this time returns the t value where the intersection occurs or -1 if there is no intersection
//     double hitSphere(const point3& center, double radius, const ray& r) {
//         // oc is the vector from the ray's origin to the spheres center 
//         vec3 oc = center - r.origin();
//         // a represents the squared length of the ray's direction vector using the dot product
//         auto a = dot(r.direction(), r.direction());
//         // b represents twice the negative dot product of the ray's direction and the vector from ray's origin to center of the sphere
//         auto b = -2.0 * dot(r.direction(), oc);
//         // c calculates the squared distnace from the ray's origin to the sphere minus the square of the radius
//         auto c = dot(oc, oc) - radius*radius;
//         // The discriminant of the quadratic equation determines where the ray intersects the sphere; if it is greater than or equal to 0 the ray intersects the sphere
//         auto discriminant = b*b - 4*a*c;       
//         // Checks if the discriminant is less than zero; meaning theres no intersection; return -1
//         if (discriminant < 0) {
//             return -1.0;
//         } else {
//         // Else return the smaller root of the quadratic equation; which represents the nearest intersection point
//             return (-b - std::sqrt(discriminant)) / (2.0 * a);
//         }
//     }

// Simplified function to determine if a given ray hits a sphere; this time returns the t value where the intersection occurs or -1 if there is no intersection
    // double hitSphere(const point3& center, double radius, const ray& r) {
    //     // oc represents the vector from the ray's origin to the sphere's center
    //     vec3 oc = center - r.origin();
    //     // a is the squraed length of the ray's direction vector which is the equivalent to the dot product of the direction vector with itself
    //     auto a = r.direction().squaredLength();
    //     // h is the dot product of the direction and the oc vector, calculating how aligned oc is with the ray's direction
    //     auto h = dot(r.direction(), oc);
    //     // c calculates the squared distance from the sphere's center to the ray's origin minus the sphere's radius squared
    //     auto c = oc.squaredLength() - radius*radius;
    //     // The discriminant of the quadratic equation determines where the ray intersects the sphere; if it is greater than or equal to 0 the ray intersects the sphere
    //     auto discriminant = h*h - a*c;

// Old ray color function to calculate the color at the end of the ray if there is intersection
    // color rayColor(const ray& r) {
    //     // Checks if the ray intersects with a sphere with a radius of 0.5 located at position 0,0,-1
    //         // if (hitSphere(point3(0,0,-1), 0.5, r))
    //         //     return color(1,0,0);

    //     // Calls the hitSphere function to check if the ray intersects with the sphere; t holds the insection point's parameter value along the ray
    //     auto t = hitSphere(point3(0,0,-1), 0.5, r);
    //     // Checks if there is a valid intersection
    //     if (t > 0.0) {
    //         // N is the normal vector at the point of intersection, pointing outward from the sphere's surface
    //         vec3 N = unitVector(r.at(t) - vec3(0,0,-1));
    //         // Converts the normal bector's components to the range of [0,1], scales them to create a color and returns it
    //         return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    //     }

    //     vec3 unitDirection = unitVector(r.direction());
    //     auto a = 0.5*(unitDirection.y() + 1.0);
    //     return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    // }

// Defines a function that returns the color of a ray r after checking for intersections with objects in the scene (world)
    // color rayColor (const ray& r, const hittable& world) {
    //     // Creates a hitRecord object rec to store details of a possible hit (intersection) between the ray and any object in the world
    //     hitRecord rec;
    //     // Checks if the ray r intersects with any object in the world.
    //     // The hit() function takes the ray, a minimum distance of 0, and a maximum distance of infinity.
    //     // If an intersection is found, it updates rec with hit details (such as point and normal) and returns true.
    //     if (world.hit(r, interval(0, infinity), rec)) {
    //         return 0.5 * (rec.normal + color(1,1,1));
    //     }

    //     vec3 unitDirection = unitVector(r.direction());
    //     auto a = 0.5*(unitDirection.y() + 1.0);
    //     return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    // }
*/

int main() {

    /* Old Test Window
        // int imageWidth = 256;
        // int imageHeight = 256;

    // Old Much Bigger Main
        // // Image
        // auto aspectRatio = 16.0 / 9.0;
        // int imageWidth = 400;
        // // Calculate the image height and ensire that its atleast 1
        // int imageHeight = int(imageWidth / aspectRatio);
        // imageHeight = (imageHeight < 1) ? 1 : imageHeight;
        // // World
        // hittableList world;
        // world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
        // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
        // // Camera 
        // auto focalLength = 1.0;
        // auto viewportHeight = 2.0;
        // auto viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);
        // auto cameraCenter = point3(0,0,0);
        // // Calculate the vectors across the horizontal and down the vertical viewport edges
        // auto viewportU = vec3(viewportWidth, 0, 0);
        // auto viewportV = vec3(0, -viewportHeight, 0);
        // // Calculate the horizontal and vertical delta vectors from pixel to pixel
        // auto pixelDeltaU = viewportU / imageWidth;
        // auto pixelDeltaV = viewportV / imageHeight;
        // // Calculate the location of the upper left pixel
        // auto viewportUpperLeft = cameraCenter - vec3(0, 0, focalLength) - viewportU/2 - viewportV/2;
        // auto pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
        // // Render
        // std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
        // // Outer loop that iterates over each row of pixels from the top to bottom
        // for (int j = 0; j < imageHeight; j++) {
        //     // clog outputs messages to the standard error stream 
        //     // \r Carriage return moves the cursor back to the start of the line allowing updates to the same line
        //     // flush forces the output buffer to display immediately ensuring the current status appears right away
        //     // std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        //     // Inner loop iterates over each pixel in the current row from left to right
        //     for (int i = 0; i < imageWidth; i++) {
        //     // Ray at the top left pixel 
        //     auto pixelCenter = pixel00_location + (i * pixelDeltaU) + (j * pixelDeltaV);
        //     auto rayDirection = pixelCenter - cameraCenter;
        //     ray r(cameraCenter, rayDirection);
        //     color pixelColor = rayColor(r, world);
        //     writeColor(std::cout, pixelColor);
        // Color Utility Function that writes a single pixel color out to the std output stream
            // auto pixelColor = color(double(i)/(imageWidth-1), double(j)/(imageHeight-1), 0);
            // writeColor(std::cout, pixelColor);
        // Old way of printing the RGB
            // // Auto deduces variable type - double for rgb
            // // Calculate r and g as fractional values between 0 and 1 based on the pixel position; and b is set to 0
            // auto r = double(i) / (imageWidth - 1);
            // auto g = double(j) / (imageHeight - 1);
            // auto b = 0.0;
            // // Multiply by 255.999 to convert the values on a range from 0 to 255(typical RGB color range) and int converts it to integer
            // int ir = int(255.999 * r);
            // int ig = int(255.999 * g);
            // int ib = int(255.999 * b);
            // // Output the RGB values for each pixel onto the console
            // std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        // }
        // }
        // // Print Done and clear any remaining text from the progress message
        // std::clog << "\rDone.                 \n";
*/
    
    // Scene Setup
    // Creates a hittableList object world, which will store all objects in the scene that can be hit by rays
    hittableList world;

    // Materials to the objects
    auto materialGround = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto materialCenter = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto materialLeft = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto materialRight = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // Add the materialed objects to the world
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, materialGround));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, materialCenter));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    world.add(make_shared<sphere>(point3(1.0, -0.0, -1.0), 0.5, materialRight));

    /* Old sphere to the world at the position (0,0,-1) with a radius of 0.5
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // Adds a large sphere with a radius of 100 to the scene centered at (0,-100.5,-1). This sphere acts as the ground
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); */

    // Camera Setup
    // Creates a camera object which wuill handle the rendering process by shooting rays into the scene
    camera cam;
    // Sets the aspect ratio of the camera to 16:9, making the output image rectangular (widescreen format)
    cam.aspectRatio = 16.0 / 9.0;
    // Sets the width of the image to 400 pixels. The height is calculated automatically based on the aspect ratio
    cam.imageWidth = 400;

    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    
    // Render Setup
    // Calls the render function on the cam object to begin rendering the scene
    // The world object is passed as a parameter, containing all the objects to render
    // The camera shoots rays from its position through each pixel in the image and checks for intersections with objects in the world to determine the color of each pixel
    cam.render(world);
}