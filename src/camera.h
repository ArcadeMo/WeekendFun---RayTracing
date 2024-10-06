#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

// Defines a camera class that handles rendering an image by shooting rays into the scene
class camera {
public:
    // Aspect ratio of the image, default is set to 1.0(square image)
    double aspectRatio = 1.0;

    // Width of the rendered image in pixels
    int imageWidth = 100;

    // Count of random samples for each pixel
    int samplesPerPixel = 10;

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
                /* Old way to color objects in the rend 
                // Calculates the center of the current pixel in 3D space
                auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
                // Computes the direction of the ray for this pixel (from the camera center to the pixel)
                auto rayDirection = pixelCenter - center;
                // Creates a ray r originating from the camera's center and pointing towards the pixel
                ray r(center, rayDirection);

                // Computes the color for the ray by determining whether it hits any objects in the world
                color pixelColor = rayColor(r, world);
                // Writes the pixel's color to the std output in the PPM format
                writeColor(std::cout, pixelColor); */
                // Initializes a color object pixelColor with all components set to 0 (black)
                color pixelColor(0,0,0);
                // Loop that iterates samplesPerPixel times to gather multiple samples for anti-aliasing; samplesPerPixel determines how many rays are shot through each pixel for more accurate color representation and smoothing
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    // Generates a new ray r for the current pixel (i,j)
                    ray r = getRay(i, j);
                    // Calls the rayColor() which returns the color for the ray after checking for intersections in the world
                    // The returned color is added to pixelColor, accumulating the color contributions from each sample
                    pixelColor += rayColor(r, world);
                }
                // Scales the accumulated pixelColor by pixelSampleScale and writes the final averaged color to the output stream in PPM format using writeColor(). The result is the final color for the pixel after multiple samples have been processed
                writeColor(std::cout, pixelSampleScale * pixelColor);
            }
        }
        // Logs a message indicating that rendering is complete
        std::clog << "\rDone.                       \n";
    }

private: 
    int imageHeight;
    // Color scale factor for a sum  of pixel samples
    double pixelSampleScale;
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

        // Calculates the scale factor used to average the color values across multiple samples per pixel
        // samplesPerPixel is the number of rays(samples) shot through each pixel and dividing 1 by this number gives the factor to scale the accumulated color values to get the average
        pixelSampleScale = 1.0 / samplesPerPixel;

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

    // Function that generates a ray to be cast through the current pixel(i,j)
    ray getRay(int i, int j) const {
        // Calls sampleSquare() to get a random offset within the pixel for anti-aliasing
        // The offset ensures that rays are slightly jittered inside the pixel to smooth out edges and create a more smooth image
        auto offset = sampleSquare();
        // Calculates the location of the current pixel sample in a 3D space
        // Takes the top left corner of the image and moves horziontally based on pixel index i and random x offset and moves vertically based on index j and random y offset
        auto pixelSample = pixel00Location
                         + ((i + offset.x()) * pixelDeltaU)
                         + ((j + offset.y()) * pixelDeltaV);
        // Sets the ray's origin to the camera's center where all rays are shot from
        auto rayOrigin = center;
        // Returns the rays direction by subtracting the ray's origin from the sample's position in 3D space
        auto rayDirection = pixelSample - rayOrigin;

        // Returns a new ray object with the calculated origin and direction
        return ray(rayOrigin, rayDirection);
    }

    // Function that returns a random offset within the square of the pixel for anti-aliasing
    vec3  sampleSquare() const {
        // Generates a random 2D vector within the range [-0.5,0.5] for both x and y directions
        // This random offset is used to jitter the ray's position within the pixel to provide anti-aliasing and smooth the rendered image
        // The z component is set to 0 as this offset is in 2D screen space
        return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
    }

    // Computes the color for a given ray r by checking for intersections with objects in the world
    color rayColor(const ray& r, const hittable& world) const {
        // Creates a hitRecord object rec to store details of a possible hit (intersection) between the ray and any object in the world
        hitRecord rec;

        // If the ray hits an object, the function returns a color based on the object's surface normal
        if (world.hit(r, interval(0, infinity), rec)) {
            // Generates a random direction vector that lies within the hemisphere oriented around the surface normal rec.normal
            // rec.normal is the normal vector at the point of intersection (rec.p), and the function randomOnHemisphere() ensures that the random direction is within the hemisphere pointing away from the surface
            vec3 direction = randomOnHemisphere(rec.normal);
            // Casts a new ray from the intersection point rec.p in the direction generated from the hemisphere around the surface normal
            // Calls the rayColor() function to compute the color of this new ray as it interacts with the world
            // The result is multiplied by 0.5 to darken the color, simulating light bouncing off the surface. This often models diffuse reflection, where rays bounce randomly off surfaces and lose energy (color intensity) with each bounce
            return 0.5 * rayColor(ray(rec.p, direction), world);
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