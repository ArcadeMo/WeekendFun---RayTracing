#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "hittable.h"

// Libraries for smart pointers and dynamic arrays
#include <vector>

// Defines hittableList as a class that inherits from hittable. Represents a list of objects that can be hit by a ray
class hittableList : public hittable {
public:

    // Objects is a vector that stores multiple shared_ptr to hittable objects. This allows the list to hold different hittable objects 
    std::vector<shared_ptr<hittable>> objects;
    
    // Default constructor that initializes an empty hittableList
    hittableList() {}

    // Constructor that takes a shared_ptr to a hittable object and adds it to the list using the add() method
    hittableList(shared_ptr<hittable> object) { add(object); }

    // Clears all objects from the objects vector
    void clear() { objects.clear(); }
    
    //Function that adds a shared_ptr to a hittable object to the objects vector
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    // Overrides the hit function from the hittable base class; checks if any object in the list is hit by the ray r within the range [raytMin, raytMax]
    bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
        // Temp hitRecord object used to store hit info for each object in the list
        hitRecord tempRec;
        // Tracks where any object has been hit by the ray
        bool hitAnything = false;
        // Stores the closest hit distance found so far, initialized to the max t ray length
        auto closestSoFar = rayT.max;

        for (const auto& object : objects) {
            // Checks if the current object is hit by the ray; if true update hitAnything to true, closestSoFar to tempRec.t(the current hit distance), and set rec to tempRec(the most recent hit record)
            if (object->hit(r, interval(rayT.min, closestSoFar), tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        // Return true if any object was hit by the ray
        return hitAnything;

     }
};

#endif