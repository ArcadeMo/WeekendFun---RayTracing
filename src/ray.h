/*

t: 1  0    1   2 
 < -  *--->*   - >
      A    B  

*/

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// Define a ray class that represents a mathematical ray with an origin and direction
class ray {
public:
    // Default constructor that creates an empty ray object
    ray() {}

    // Parameterized constructor that initializes the ray with a given origin and direction
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    // Getter functions that returns a reference to the ray's origin and direction
    const point3& origin()   const  { return orig; }
    const vec3& direction()  const  { return dir; }

    // Function that calculates a point along the ray at a distance t from the origin in the direction of the ray
    point3 at(double t) const {
        // Returns the point reached by traveling t unites along the ray's direction
        return orig + t*dir;
    }

// Private variables that can only be accessed or modified by member functions of the ray
private:
    point3 orig;
    vec3 dir;
};

#endif