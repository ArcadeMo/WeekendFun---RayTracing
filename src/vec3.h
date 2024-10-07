#ifndef VEC3_H
#define VEC3_H

// Class to represent a 3D vector
class vec3 {
    public: 
    // Array that holds x,y,z of the vector
    double e[3];

    // Constructor that initializes the vector to (0,0,0)
    vec3() : e{0,0,0} {}

    // Parameterized constructor that initializes the vector 
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // Functions to access x,y,z 
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Overloads the - operator to return a vector with all components negated
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    // Overloads the [] operator for read-only and read-write access to elements
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    // Overloads += to add another vector to this one component-wise
    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Overloads *= to scale the vector by multiplying by a scalar
    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] += t;
        return *this;
    }

    // Overloads /= to scale the vector by dividing by a scalar.
    vec3& operator /= (double t) {
        return *this *= 1/t;
    }

    // Returns the vector magnitude
    double length() const {
        return std::sqrt(squaredLength());
    }

    // Returns the squared length using the Pythagorean theorem
    double squaredLength() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // Checks if the vector is close to zero in all dimensions(x,y,z)
    bool nearZero() const {
        // Sets a small threshold value s to define "near zero"
        auto s = 1e-8;
        // Uses the absolute value function fabs to check if each component of the vector is smaller than s; returns true if all components are nearly zero
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    // (First Overload) Function that generates a random vec3 object where each component (x,y,z) is a random double value between 0 and 1; static so can't be called outside vec3
    static vec3 random() {
        // Each call to randomDouble() returns a value between 0.0 and 1.0, so 3 random components
        return vec3(randomDouble(), randomDouble(), randomDouble());
    }

    // (Second Overload) This overload generates a random vec3 object where each component is a random double value with a specified range [min,max]
    // The range for the random numbers is determined by the min and max parameters
    static vec3 random(double min, double max) {
        // Each call returns a random value between min and max
        return vec3(randomDouble(min,max), randomDouble(min,max), randomDouble(min,max));
    }
};

// Create an alias for vec3 for geometric clarity 
using point3 = vec3;

// Vector Utility Functions

// Overloads << to allow printing vec3 objects to the console
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' <<v.e[2];
}

// Overloads +, -, *, / for vector addition, subtraction, multiplication, and scalar division
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

// Calculates the dot product, good to know how parallel two vectors are
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
    + u.e[1] * v.e[1]
    + u.e[2] * v.e[2];
}

// Calculates the cross product, producing a vector perpendicular to u and v
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Returns a unit vector (vector with length 1) pointing in the same direction as v 
inline vec3 unitVector(const vec3& v) {
    return v / v.length();
}

// Function generates a random vec3 that lies within or on the surface of a unit sphere, the vector is returned as a unit vector(normalized to a length of 1)
inline vec3 randomUnitVector() {
    // Loop that continuously generates random vectors until one is found that satisfies the condition to be inside the unit sphere
    while (true) {
        // Calls the random function to generate a random vector p; ensures that the vector lies within the cube that ranges (-1,-1,-1) to (1,1,1)
        auto p = vec3::random(-1,1);
        // Calculates the squared length of vector p which is the same as taking the dot product of the vector with itself ((p.x^2 + p.y^2 + p.z^2) 
        auto lensQ = p.squaredLength();
        // Exclude near-zero vectors to avoid numerical instability during normalization
        if (1e-160 < lensQ && lensQ <= 1)
        // If the vector p satisfies the condition, it is normalized to have a length of exactly 1 by dividing it by its actual length (sqrt(lensQ)), the function returns the normalized unit vector, ensuring it lies on the surface of the unit sphere.
            return p / sqrt(lensQ);
    }
}

// Function that generates a random vec3 that lies on the hemisphere oriented around a given normal vector
inline vec3 randomOnHemisphere(const vec3& normal) {
    // Calls the randomUnitVector function to generate a random unit vector onUnitSphere on the surface
    vec3 onUnitSphere = randomUnitVector();
    // Computes the dot product of the random vector and the given normal
    // The dot product checks whether the angle between the vectors is less than 90 degrees(random vector points in the same general direction as the normal)
    // If the dot product is positive, the random vector lies in the same hemisphere as the normal
    if (dot(onUnitSphere, normal) > 0.0)
        // If the random vector is in the same hemisphere as the normal, return the vector as is
        return onUnitSphere;
    else
        // Else flip the vector by negating it, ensuring it lies in the hemisphere aligned with the normal 
        return -onUnitSphere;
}

// Defines a function that calculates the reflection of vector v off a surface with normal n
inline vec3 reflect(const vec3& v, const vec3& n) {
    // Reflection formula: subtracts twice the projection of v onto n from v resulting in the reflected vector
    return v - 2 * dot(v,n) * n;
}

// Defines a function to caluclate the refraction of a vector uv as it passes through a surface with a normal n, based on the ration of refractive indices (etaiOverEtat)
inline vec3 refract(const vec3& uv, const vec3& n, double etaiOverEtat) {
    // Computes the cosine of the angle between the incoming vector uv and the surface normal n, fmin ensures the result does not exceed 1, clamping the value for stability
    auto cosTheta = std::fmin(dot(-uv, n), 1.0);
    // Calculates the perpendicular component (rPerp) of the refracted ray using Snell's law
    // etaiOverEtat scales the refracted direction based on the refractive indices
    vec3 rPerp =  etaiOverEtat * (uv + cosTheta * n);
    // Calculates the parallel component (rParallel) of the refracted ray; ensures the total length of the refracted ray is 1 by subtracting the squared length of rPerp from 1.0, taking the square root to find the parallel component
    vec3 rParallel = -std::sqrt(std::fabs(1.0 - rPerp.squaredLength())) * n;
    // Returns the sum of the perpendicular and parallel components, forming the complete refracted ray. This vector represents the direction of the light as it passes through the surface
    return rPerp + rParallel;
}

// Generates a random 2D vector inside a unit disk (circle with radius 1 in the x-y plane)
inline vec3 randomInUnitDisk() {
    // Continuously generates random vectors until one falls inside the unit disk
    while(true) {
        // Creates a random 2D vector p with x and y values between -1 and 1 and a z value of 0 (restricted to the x-y plane)
        auto p = vec3(randomDouble(-1,1), randomDouble(-1,1), 0);
        // Checks if the vector's squared length is less than 1, ensuring it lies within the unit disk
        if(p.squaredLength() < 1)
            // Returns the vector p once it meets the condition of being inside the unit disk
            return p;
    }
}

#endif