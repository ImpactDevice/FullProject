#include "PolynomialLeastSquares.h"
#include <stdexcept>
#include <cmath>

PolynomialLeastSquares::PolynomialLeastSquares(int degree) : degree(degree) {}

Eigen::VectorXd PolynomialLeastSquares::fit(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size() || x.empty()) {
        throw std::invalid_argument("Input arrays must have the same non-zero length.");
    }

    int n = x.size();
    Eigen::MatrixXd A(n, degree + 1);
    Eigen::VectorXd b(n);

    // Fill the matrix A and vector b
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= degree; ++j) {
            A(i, j) = std::pow(x[i], j);
        }
        b(i) = y[i];
    }

    // Solve for the coefficients using the normal equation
    Eigen::VectorXd coefficients = (A.transpose() * A).ldlt().solve(A.transpose() * b);
    return coefficients;
}