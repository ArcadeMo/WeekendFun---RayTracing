#ifndef INTERVAL_H
#define INTERVAL_H

// Defines an interval class that represents a range of values between min and max
class interval {
public:
    // Two member variables that represents the lower and upper bounds of the interval
    double min, max;

    // Default constructor that creates an empty interval where min is positive infinity and max is negative infinity to indicate that the interval is invalid or contains no values
    interval() : min(+infinity), max(-infinity) {} 

    // Parameterized constructor that initializes the interval with specific min and max values
    interval(double min, double max) : min(min), max(max) {}

    // Member function that returns the size of the interval by calculating max - min
    double size() const {
        return max - min;
    }

    // Member function that checks the value x is within the interval including the boundary points
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // Member function that checks if the value x is strictly inside the interval excluding the boundary points
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // Declares two static constants for commonly used intervals
    static const interval empty, universe;
};

// Defines the empty interval as having a range from positive infinity to negative infinity; no valid range
const interval interval::empty =    interval(+infinity, -infinity);

// Defines the universe interval covering all possible values from negative infinity to positive infinity
const interval interval::universe = interval(+infinity, -infinity);

#endif