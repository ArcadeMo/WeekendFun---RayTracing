#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

// Defines a sphere class that inherits from hittable, making it a specific type of object that a ray can hit
class sphere : public hittable {
public:
    // Constructor initializes center and radius of the sphere; uses fmax which returns the maximum of two floating point arguements, ensures the radius is non-negative
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    // Overrides the hit function from the hittable base class to determine if the ray hits the sphere
    bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
        // oc is the vector from the ray's origin to the spheres center 
        vec3 oc = center - r.origin();
        // a is the squared length of the ray's direction vector
        auto a = r.direction().squaredLength();
        // h is the dot product of the ray's direction and oc representing their alignment
        auto h = dot(r.direction(), oc);
        // c is te squared length of the oc ray minus the square of the sphere's radius
        auto c = oc.squaredLength() - radius*radius;
        // Discriminant checks if there'es an intersection; if negative no hit occurs - return false
        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        // Computes the square root of the discriminant for finding the intersection points
        auto sqrtD = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range (t value)
        auto root = (h - sqrtD) / a;
        // Check if the root is outside the valid range, if so try another root
        if (!rayT.surrounds(root)) {
            // Calculates the second possible intersection
            root = (h + sqrtD) / a;
            // If this root is also out of range, return false since there is not valid intersection
            if (!rayT.surrounds(root))
                return false;
        }

        // Sets rec.t to the valid intersection point t
        rec.t = root;
        // Calculates the intersection point p using the ray function r.at(t)
        rec.p = r.at(rec.t);

        // Calculate the outward normal vector at the intersection point of rec.p; dividing by the radius normalizes the vectore making it a unit vector that points outward from the sphere radius
        vec3 outwardNormal = (rec.p - center) / radius;

        // Calls setFaceNormal on the hit record to determine where the ray hit the front or back face of the sphere; passes the ray and the outward normal to set the correct normal for the intersection based on the ray's direction
        rec.setFaceNormal(r, outwardNormal);
        // Calculate the normal vector at the intersection point, pointing outward from the sphere's surface
            // rec.normal - (rec.p - center) / radius;

        // Return true indicating a valid intersection occured 
        return true;
    }
private:
    point3 center;
    double radius;
};

#endif