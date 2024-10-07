#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

// An abstract base class that represents a material for objects in the ray tracing system
// Materials define how rays interact with objects - whether they reflect, refract, or absorb light
class material {
public:
    // A virtual destructor allowing proper cleanup when derived material classes are deleted; default tells the compiler to generate the default destructor
    virtual ~material() = default;

    // Abstract method (can be overridden by derived classes) that describes how the material scatters incoming rays 
    // returns true if the ray is scattered(reflected or refracted)
    virtual bool scatter(
        // rIncoming is the incoming ray
        // rec contains info about the intersection point, including the surface normal and the points of impact
        // attenuation represents how much light is absorbed by the material during scattering, affecting the birghtness of the scattered ray
        // scattered ray that results from interaction with the surface (reflect or refract)
        const ray& rIncoming, const hitRecord& rec, color& attenuation, ray& scattered
    ) const {
        // By default the function returns false, meaning the ray is absorbed or does not scatter
        return false;
    }
};

// Class that represents a Lambertian(diffuse) material, which scatters light equally in all directions. It inherits from the material class, meaning it must implement the scatter function
class lambertian : public material {
public: 
    // Constructor that initializes the material's albedo(the material's base color)
    lambertian(const color& albedo) : albedo(albedo) {}

    // Overrides the scatter function from the material class to describe how an incoming ray interacts with the Lambertian material, scattering light in random directions
    bool scatter(const ray& rIncoming, const hitRecord& rec, color& attenuation, ray& scattered)
    const override {
        // Generates a random direction for the scattered ray, rec.normal is the surface normal at the hit point and randomUnitVector adds a random unit vector to the surface normal, ensuring the scattered ray is in a random direction that favors the hemisphere around the normal
        auto scatterDirection = rec.normal + randomUnitVector();

        // Catch degenerate scatter direction
        if (scatterDirection.nearZero())
            // If the scatter direction is near zero, it is set to the surface normal rec.normal, ensuring the scattered ray has a valid direction
            scatterDirection = rec.normal;

        // Creates a scattered ray using the hit point rec.p as the origin and scatterDirection as the direction, represents the light that bounces off the surface in a new random direction
        scattered = ray(rec.p, scatterDirection);
        // Sets the attenuation(how much light is reflected) to the materials albedo which controls how much light is scattered versus absorbed
        attenuation = albedo;
        // return true if the ray was successfully scattered
        return true;
    }

private: 
    // A color that represents how much light the material reflects. For exmaple, an albedo of color(0.5, 0.3, 0.3) would reflect 50% red, 30% green and blue light
    color albedo;
};

// Defines a metal material class that inherits from material, representing reflective surfaces like metal
class metal : public material {
public:
    // Constructor that initializes the materia's albedo, which controls the color of the reflected light, fuzz is clamped to a max value of 1 ensuring fuzziness stays within reasonable range
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    // Implements scatter for a metal surface
    bool scatter(const ray& rIncoming, const hitRecord& rec, color& attenuation, ray& scattered)
    const override {
        // Calculates the reflection of the incoming ray direction based on the surface normal
        vec3 reflected = reflect(rIncoming.direction(), rec.normal);
        // Adds a random fuzziness to the reflacted vector
        reflected = unitVector(reflected) + (fuzz * randomUnitVector());
        // Sets the scattered ray to start at the hit point and travel in the reflected direction
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        // Returns true if the scattered ray's direction is in the same hemisphere as the surface normal(the reflection is valid); ensures that the ray doesn't scatter into the surface but rather reflects outward
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private: 
    color albedo;
    // Fuzz value determines how "blurry" the reflections are
    double fuzz;
};

// This class represents a dielectric material, such as glass or water, which refracts light
class dielectric : public material {
public:
    // Constructor that initializes the material with a specific refractionIndex, which represents how light bends when passing through the material
    dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

    bool scatter(const ray& rIncoming, const hitRecord& rec, color& attenuation, ray& scattered)
    const override {
        // Sets attenuation to white, meaning the material does not absorb light; it only refracts or reflects it
        attenuation = color(1.0, 1.0, 1.0);
        // Determines the ratio of refractive indices (ri) based on whether the ray is entering or exiting the material
        // If the ray is hitting the front face of the surface, ri is set to the inverse of the refractionIndex. Otherwise, it uses the material’s refractionIndex.
        double ri = rec.frontFace ? (1.0/refractionIndex) : refractionIndex;
        // Normalizes the incoming ray’s direction, creating a unit vector unitDirection
        vec3 unitDirection = unitVector(rIncoming.direction());
        
        // Calculates the cosine of the angle between the negative of the incoming ray direction (-unitDirection) and the surface normal (rec.normal)
        // Uses std::fmin to clamp the result to a maximum of 1.0, ensuring numerical stability
        double cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
        // Calculates the sine of the angle using the Pythagorean identity: sin^2(θ) + cos^2(θ) = 1. This is used to determine the possibility of refraction
        double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);

        // Checks if total internal reflection occurs by comparing ri * sinTheta to 1.0
        // If ri * sinTheta > 1.0, refraction cannot happen, and the ray will be reflected
        bool cannotRefract = ri * sinTheta > 1.0;
        // Declares a variable direction to store the final direction of the scattered ray (either reflected or refracted)
        vec3 direction;

        // If total internal reflection occurs (cannotRefract is true), the ray will reflect instead of refract; even if refraction is possible, this checks if the reflectance (based on angle and refractive index) is high enough to cause reflection probabilistically (using random_double() to determine randomness)
        if (cannotRefract || reflectance(cosTheta, ri) > randomDouble())
            // Sets the ray direction to the reflected direction using the reflect() function, which calculates the reflection based on the incoming direction and surface normal
            direction = reflect(unitDirection, rec.normal);
        else
            // Else set the direction by calculating the refracted ray direction using Snell's Law, based on the incoming ray's direction, the surface normal, and the refractive index ratio ri
            direction = refract(unitDirection, rec.normal, ri);

        // Constructs the scattered ray starting at the intersection point (rec.p) and traveling in the chosen direction (either reflected or refracted)
        scattered = ray(rec.p, direction);
        // Returns true to indicate the ray has been successfully refracted by the dielectric material
        return true;
    }

private:
    // The refractive index of the material, which determines how much the light bends when entering or exiting the material 
    double refractionIndex;

    // A static function that calculates the reflectance of light using Schlick’s approximation; cosine is the cosine of the angle between the incoming ray and the surface normal
    static double reflectance(double cosine, double refractionIndex) {
    // Use Schlick's approximation for reflectance
    // Calculates the base reflectance r0 when the incoming light is perpendicular to the surface
    // This formula is based on the ratio of refractive indices of the two media
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    // Squares r0 to obtain the reflectance at normal incidence (angle = 0)
    r0 = r0*r0;
    // Uses Schlick's approximation to adjust the reflectance based on the angle of incidence (represented by cosine) 
    // The higher the angle (closer to grazing), the more reflective the surface becomes
    // The term std::pow((1 - cosine), 5) makes the reflectance approach 1 as the angle increases
    return r0 + (1-r0)*std::pow((1 - cosine), 5);
}
};


    // Derived material classes like diffuse or reflective materials will override this function to implement specific scattering behaviors
#endif