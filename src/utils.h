#ifndef UTILS_H
#define UTILS_H

// Includes necessary standard libraries for:
// mathematical functions (<cmath>)
// input/output operations (<iostream>) 
// handling limits of data types (<limits>)
// smart pointers (<memory>)
#include <cmath>
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

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif