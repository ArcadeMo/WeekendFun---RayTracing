#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

// Defines a camera class that handles rendering an image by shooting rays into the scene
class camera {
public:
    // Aspect ratio of the image, default is set to 1.0(square image)
    double aspectRatio = 1.0;

    // Width of the rendered image in pixels
    int imageWidth = 100;

    // Count of random samples for each pixel
    int samplesPerPixel = 10;

    // Maximum number of ray bounces into scene
    int maxDepth = 10; 

    // Vertical view angle (Field of view)
    double vfov = 90;

    // Point camera is looking from
    point3 lookFrom = point3(0,0,0);

    // Point camera is looking at
    point3 lookAt = point3(0,0,-1);

    // Camera-relative "up" direction
    vec3 vup = vec3(0,1,0);

    // Variation angle of rays through each pixel
    double defocusAngle = 0;

    // Distance from camera lookfrom point to plane of perfect focus
    double focusDist = 0;

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
                    pixelColor += rayColor(r, maxDepth, world);
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
    // Camera frame basis vector
    vec3 u, v, w;
    // Defocus disk horizontal radius
    vec3 defocusDiskU;
    // Defocus disk vertical radius
    vec3 defocusDiskV;

    // Initialize function sets up the camera parameters, including the image size, pixel locations, and fov
    void initialize() {
        // Calculates the height of the image based on the width and aspect ratio
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        // Calculates the scale factor used to average the color values across multiple samples per pixel
        // samplesPerPixel is the number of rays(samples) shot through each pixel and dividing 1 by this number gives the factor to scale the accumulated color values to get the average
        pixelSampleScale = 1.0 / samplesPerPixel;

        // Sets the camera's position at the origin (0,0,0)
        center = lookFrom;

        // Sets the focal length of the camera, which determines how far objects are from the camera
            // auto focalLength = (lookFrom - lookAt).length();
        // Converts the vertical field of view (vfov) from degrees to radians, storing it in theta
        auto theta = degreesToRadians(vfov);
        // Calculates the half-height of the viewport by taking the tangent of half the vertical field of view (theta / 2)
        auto h = std::tan(theta/2);
        // Sets the height of the camera's viewport (the visible area); computes the full height of the viewport by multiplying 2 * h by the focal length, which adjusts the height based on the camera's focal length.
        auto viewportHeight = 2 * h * focusDist;
    
        // Calculates the width of the viewport based on the aspect ratio
        auto viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame
        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        // Defines the horizontal direction of the viewport(width-wise); vector across viewport horizontal edge
        auto viewportU = viewportWidth * u;
        // Defines the vertical direction of the viewport(height-wise); vector down viewport vertical edge
        auto viewportV = viewportHeight * -v;

        // Calculates the horizontal spacing between pixels
        pixelDeltaU = viewportU / imageWidth;
        // Calculates the vertical spacing between pixels
        pixelDeltaV = viewportV / imageHeight;

        // Determines the location of the first pixel in the top left corner of the image
        auto viewportUpperLeft =  center - (focusDist * w) - viewportU/2 - viewportV/2;
        pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        // Calculate the camera defocus disk basis vectors (the area simulating camera lens blur)
        auto defocusRadius = focusDist * std::tan(degreesToRadians(defocusAngle / 2));

        // Scales the camera's horizontal and vertical vectors by the defcus radius to get the basis vector for the defocus disk
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
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
        // Sets the ray's origin to the camera's lens where all rays are shot from
        auto rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
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

    // Function returns a random point within the camera's defocus disk, simulating depth of field blur
    point3 defocusDiskSample() const {
        // Generates a random point p inside a unit disk using the randomInUnitDisk();
        auto p = randomInUnitDisk();
        // Calculates the sampled point in the defocus disk by scaling p's x and y components by the defocus disk's horizontal (defocusDiskU) and vertical (defocusDiskV) vectors, respectively, and then adds this to the camera's center
        // This simulates a random offset within the defocus disk for depth-of-field effects
        return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    // Computes the color for a given ray r by checking for intersections with objects in the world
    color rayColor(const ray& r, int depth, const hittable& world) const {
        // Check if we exceeded the ray bounce limit, no more light is gathered, return black if it does
        if (depth <= 0)
            return color(0,0,0);

        // Creates a hitRecord object rec to store details of a possible hit (intersection) between the ray and any object in the world
        hitRecord rec;

        // If the ray hits an object, the function returns a color based on the object's surface normal
        if (world.hit(r, interval(0.001, infinity), rec)) {
            /* Generates a random direction vector that lies within the hemisphere oriented around the surface normal rec.normal
            // rec.normal is the normal vector at the point of intersection (rec.p), randomUnitVector function randomly generating a vector according to Lambertian distribution, and the function randomOnHemisphere() ensures that the random direction is within the hemisphere pointing away from the surface
            vec3 direction = rec.normal + randomUnitVector();
            // Casts a new ray from the intersection point rec.p in the direction generated from the hemisphere around the surface normal
            // Calls the rayColor() function to compute the color of this new ray as it interacts with the world
            // The result is multiplied by 0.1 to darken the color, simulating light bouncing off the surface. This often models diffuse reflection, where rays bounce randomly off surfaces and lose energy (color intensity) with each bounce
            return 0.1 * rayColor(ray(rec.p, direction), depth - 1, world);*/
            // Declares a scattered ray which will store the ray after it interacts with the material
            ray scattered;
            // Declares a color variable which stores how much light is absorbed or reflected by the material
            color attenuation;
            // If scatter returns true it updates the above variables
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                // Recrusively calls rayColor for the scattered ray reducing the depth(the remaining ray bounce count)
                // Multiplies the resulting color by attenuation to apply the material's reflectivity or absorption
                return attenuation * rayColor(scattered, depth - 1, world);
            // Return black if the ray isn't scattered indicating no light is reflected
            return color(0,0,0);
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