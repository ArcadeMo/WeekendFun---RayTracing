#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

// Defines a camera class that handles rendering an image by shooting rays into the scene
class camera {
public:
    // Aspect ratio of the image, default is set to 1.0(square image)
    double aspectRatio = 1.0;

    // Width of the rendered image in pixels
    int    imageWidth = 100;

    // Main rendering function that generates the image by shooting rays into the world, takes a reference to the hittable world(contains all objects in the scene)
    void render(const hittable& world) {
        // Calls a helpher function to set up the camera parameters before rendering begins
        initialize();

        // Outputs the header for the PPM image format(plain text format for storing images)
        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        // Nested loop for rendering
        // Outer loop that iterates over each row (scanline) of the image from top to bottom; logs the number of remaining scanlines to the standard error using std::clog
        for (int j = 0; j < imageHeight; j++) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            // Inner loop that iterates over each picel in the current row from left to right
            for (int i = 0; i < imageWidth; i++) {
                // Calculates the center of the current pixel in 3D space
                auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
                // Computes the direction of the ray for this pixel (from the camera center to the pixel)
                auto rayDirection = pixelCenter - center;
                // Creates a ray r originating from the camera's center and pointing towards the pixel
                ray r(center, rayDirection);

                // Computes the color for the ray by determining whether it hits any objects in the world
                color pixelColor = rayColor(r, world);
                // Writes the pixel's color to the std output in the PPM format
                writeColor(std::cout, pixelColor);
            }
        }
        // Logs a message indicating that rendering is complete
        std::clog << "\rDone.                       \n";
    }

private: 
    int imageHeight;
    point3 center;
    // 3D coordinates of the upper left corner of the image's first pixel
    point3 pixel00Location;
    // Horizontal offset between adjacent pixels in the image
    vec3 pixelDeltaU;
    // Vertical offset between adjacent pixels in the image
    vec3 pixelDeltaV;

    // Initialize function sets up the camera parameters, including the image size, pixel locations, and fov
    void initialize() {
        // Calculates the height of the image based on the width and aspect ratio
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        // Sets the camera's position at the origin (0,0,0)
        center = point3(0,0,0);

        // Sets the focal length of the camera, which determines how far objects are from the camera
        auto focalLength = 1.0;
        // Sets the height of the camera's viewport (the visible area)
        auto viewportHeight = 2.0;
        // Calculates the width of the viewport based on the aspect ratio
        auto viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);

        // Defines the horizontal direction of the viewport(width-wise)
        auto viewportU = vec3(viewportWidth, 0, 0);
        // Defines the vertical direction of the viewport(height-wise)
        auto viewportV = vec3(0, -viewportHeight, 0);

        // Calculates the horizontal spacing between pixels
        pixelDeltaU = viewportU / imageWidth;
        // Calculates the vertical spacing between pixels
        pixelDeltaV = viewportV / imageHeight;

        // Determines the location of the first pixel in the top left corner of the image
        auto viewportUpperLeft =  center - vec3(0,0, focalLength) - viewportU/2 - viewportV/2;
        pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    // Computes the color for a given ray r by checking for intersections with objects in the world
    color rayColor(const ray& r, const hittable& world) const {
        // Creates a hitRecord object rec to store details of a possible hit (intersection) between the ray and any object in the world
        hitRecord rec;

        // If the ray hits an object, the function returns a color based on the object's surface normal
        if (world.hit(r, interval(0, infinity), rec)) {
            // Calculates the hit color as a shade based on the surface normal
            return 0.5 * (rec.normal + color(1,1,1));
        }

        // Computes the unit vector of the ray direction
        vec3 unitDirection = unitVector(r.direction());
        // Computes a blending factor based on the y-component of the ray's direction
        auto a = 0.5*(unitDirection.y() + 1.0);
        // Blends between white and sky blue based on the ray's direction to create a sky gradient
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif