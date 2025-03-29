#include "PolynomialLeastSquares.h"
#include <iostream>
#include <vector>
#include <Eigen/Dense>

// Function to calculate the derivative of the polynomial
Eigen::VectorXd calculateDerivative(const Eigen::VectorXd& coefficients) {
    int degree = coefficients.size() - 1;
    Eigen::VectorXd derivativeCoefficients(degree);

    for (int i = 1; i <= degree; ++i) {
        derivativeCoefficients(i - 1) = coefficients(i) * i;
    }

    return derivativeCoefficients;
}

// Function to evaluate the polynomial at a given point
double evaluatePolynomial(const Eigen::VectorXd& coefficients, double x) {
    double result = 0.0;
    for (int i = 0; i < coefficients.size(); ++i) {
        result += coefficients(i) * std::pow(x, i);
    }
    return result;
}

int main() {
    // Example data
    std::vector<double> times = {0, 1, 2, 3, 4, 5};
    std::vector<double> distances = {0, 1, 4, 9, 16, 25};

    // Degree of the polynomial
    int degree = 2;

    // Create an instance of the PolynomialLeastSquares class
    PolynomialLeastSquares pls(degree);

    // Fit the polynomial to the data
    Eigen::VectorXd coefficients = pls.fit(times, distances);

    // Print the coefficients of the fitted polynomial
    std::cout << "Fitted polynomial coefficients: " << coefficients.transpose() << std::endl;

    // Calculate the derivative of the polynomial
    Eigen::VectorXd derivativeCoefficients = calculateDerivative(coefficients);

    // Print the coefficients of the derivative
    std::cout << "Derivative polynomial coefficients: " << derivativeCoefficients.transpose() << std::endl;

    // Evaluate the polynomial and its derivative at a specific point
    double x = 2.0;
    double y = evaluatePolynomial(coefficients, x);
    double dy = evaluatePolynomial(derivativeCoefficients, x);

    std::cout << "Polynomial value at x = " << x << ": " << y << std::endl;
    std::cout << "Derivative value at x = " << x << ": " << dy << std::endl;

    return 0;
}