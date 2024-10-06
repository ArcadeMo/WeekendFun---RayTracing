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

    // Function that clamps the value x within the range [min,max]; if x is outside the range, it will be set to either min or max value
    double clamp(double x) const {
        // If x is less than the min value, the function returns min,  clamping x to the lower bound
        if (x < min) return min;
        // If x is greater than max value, the function returns max, clamping x to the upper bound
        if (x > max) return max;
        // If x is within the range [min,max], the function returns x without modifiying it
        return x;
    }

    // Declares two static constants for commonly used intervals
    static const interval empty, universe;
};

// Defines the empty interval as having a range from positive infinity to negative infinity; no valid range
const interval interval::empty =    interval(+infinity, -infinity);

// Defines the universe interval covering all possible values from negative infinity to positive infinity
const interval interval::universe = interval(+infinity, -infinity);

#endif