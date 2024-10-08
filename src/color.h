#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

// Function converts a linear color component to its gamma-corrected value
// Gamma correction is applied to linear color values to adjust brightness and make them display correctly on screens
inline double linearToGamma(double linearComponent) {
    // Check if the linear component is greater than 0 since gamma correction is only applied to positive values
    if (linearComponent > 0)
        // If positive, apply gamma correction by returning the square root of the component, assumed the gamma value is 2.0
        return std::sqrt(linearComponent);
    // If its 0 or negative the result is 0
    return 0;
}

// Defines the writeColor function that writes a color (which is a vec3) to an output stream
// std::ostream& out is parameter for the output stream where the color will be written (e.g., std::cout)
// const color& pixelColor is a constant reference to the color to avoid copying
void writeColor(std::ostream& out, const color& pixelColor) {
    // Retrieves the red, green, and blue components from pixelColor
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Apply a linear to gamma transform for gamma 2
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);
    
    // Translate the [0,1] component values to the byte range [0,255] for image colors
    // The range is used to clamp color intensity values; ensuring they stay within a valid range for color representation
    static const interval intensity(0.000, 0.999);
    // Multiply the clamped value by 255.999 to scale it to an integer in the range [0, 255], which is the standard range for color in 8-bit RGB format
    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif