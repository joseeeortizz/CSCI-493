/*
Name: Jose Miguel Ortiz
Date: 04/06/2025
Email: jose.ortiz60@myhunter.cuny.edu

*/
#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <functional>

// Computes the definite integral of a given function `func` over the interval [x1, x2]
// using a specified precision. This function is parallelized with OpenMP.
double Integral(std::function<double(double)> func, double x1, double x2, double precision);

#endif // INTEGRAL_H