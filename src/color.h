#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

// Defines the writeColor function that writes a color (which is a vec3) to an output stream
// std::ostream& out is parameter for the output stream where the color will be written (e.g., std::cout)
// const color& pixelColor is a constant reference to the color to avoid copying
void writeColor(std::ostream& out, const color& pixelColor) {
    // Retrieves the red, green, and blue components from pixelColor
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Translate the [0,1] component values to the byte range [0,255] for image colors
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif