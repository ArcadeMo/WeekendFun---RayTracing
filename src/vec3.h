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

#endif