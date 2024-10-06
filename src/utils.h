#ifndef UTILS_H
#define UTILS_H

// Includes necessary standard libraries for:
// mathematical functions (<cmath>)
// input/output operations (<iostream>) 
// handling limits of data types (<limits>)
// smart pointers (<memory>)
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

// Simplifies the usage of make_shared and shared_ptr by removing the need to prefix them with std::
using std::make_shared;
using std::shared_ptr;

// Defines a constant infinity to represent positive infinity using the numeric_limits library
const double infinity = std::numeric_limits<double>::infinity();

// Defines the constant pi to a high degree of precision for use in trigonometric calculations
const double pi = 3.1415926535897932385;

// Converts an angle in degrees to radians by multipling the input by pi and divides by 180 to get the radian equivalent
inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

// Function that returns a random double value between 0 and 1
inline double randomDouble() {
    // Uses the rand() function(which generates a randon int) and divides it by RAND_MAX + 1.0 to scale the result to the range [0.0,1.0]
    // RAND_MAX is a constant representing the max value that rand() can return
    return std::rand() / (RAND_MAX + 1.0);
}

// Function that returns a random double value within the range [min,max]
inline double randomDouble(double min, double max) {
    // Calculates the random value within the specified range
    // Random double generates a random value between 0.0 and 1.0
    // Expression (max-min) scales this value to the desired range and adding min shifts the result at the min value
    // Final result is a random double between min and max(not including max)
    return min + (max - min) * randomDouble();
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif