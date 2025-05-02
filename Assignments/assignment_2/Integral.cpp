/*
Name: Jose Miguel Ortiz
Date: 04/06/2025
Email: jose.ortiz60@myhunter.cuny.edu

*/

#include "Integral.h"
#include <omp.h>    // OpenMP header for parallelism
#include <cmath>    // For mathematical functions (optional here, but good to include)

// This function uses the trapezoidal rule for numerical integration.
// It parallelizes the summation step using OpenMP to speed up computation.
double Integral(std::function<double(double)> func, double x1, double x2, double precision) {
    // Calculate the number of small intervals (steps) we will divide the range into.
    // More intervals mean more precise approximation.
    long long num_steps = static_cast<long long>((x2 - x1) / precision);

    // Calculate the width of each step.
    double step = (x2 - x1) / num_steps;

    // Initialize the total sum (the final integral value) to 0.
    double sum = 0.0;

    // Parallelize the loop using OpenMP.
    // Each thread will compute a portion of the total sum.
    // The 'reduction(+:sum)' ensures all partial sums are safely added together.
#pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < num_steps; ++i) {
        // x_left is the left boundary of the current interval
        double x_left = x1 + i * step;

        // x_right is the right boundary of the current interval
        double x_right = x_left + step;

        // Compute the area of the trapezoid under the curve for this small interval
        double area = 0.5 * (func(x_left) + func(x_right)) * step;

        // Add this small area to the total sum
        sum += area;
    }

    // Return the computed integral value
    return sum;
}