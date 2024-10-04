#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// Defines a class to store informatiuon about a ray-object intersection
class hitRecord { 
public: 
    // p stores the point of intersection where the ray hits an object
    point3 p;
    // normal represents the surface normal at the intersection point
    vec3 normal;
    // t stores the t parameter value along the ray where the intersection occurs; which can be used to calculate the exact hit point
    double t;
    // bool variable indicating whether the intersection point lies on the front(outer) side of the surface relative to the ray's direction
    bool frontFace;

    // Function to determine whether the ray hit the front or back face of the surface; parameters are the ray and the outward normal which is a normal vector pointing outwards from the surface
    void setFaceNormal(const ray& r, const vec3& outwardNormal) {
        // Set front face to true if the ray is hitting the surface from the outside
        // Use the dot product to check the angle between the ray's direction and the outward normal; if < 0 the angle is greater than 90 degrees meaning the ray hits from the outside
        frontFace = dot(r.direction(), outwardNormal) < 0;
        // Set the normal of the hit record, use the positive outward normal if front face is true; meaning the ray hits the front, otherwise use the negative meaning the front face is false and the ray hits the back
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

// Defines an abstract base class representing objects tha can be hit by a ray
class hittable {
public:
    // a virtual destructor ensuring that dervived classes clean up properly. = default specifies the compilier-generated destructor
    virtual ~hittable() = default;

    // A pure virtual function that derived classes must implement
    // Determines if a ray hits the object within range [raytMin, raytMax]
    // hitRecord stores the intersection details if a hit occurs
    // const = 0 makes hittable an abstract class, meaning you can't instantiate it directly but can derive other classes from it that implement hit()
    virtual bool hit(const ray& r, double raytMin, double raytMax, hitRecord& rec) const = 0;
};

#endif