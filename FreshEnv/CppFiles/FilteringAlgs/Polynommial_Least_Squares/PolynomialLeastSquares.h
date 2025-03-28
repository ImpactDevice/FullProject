#ifndef POLYNOMIAL_LEAST_SQUARES_H
#define POLYNOMIAL_LEAST_SQUARES_H

#include <vector>
#include <Eigen/Dense>

class PolynomialLeastSquares {
public:
    PolynomialLeastSquares(int degree);
    Eigen::VectorXd fit(const std::vector<double>& x, const std::vector<double>& y);

private:
    int degree;
};

#endif // POLYNOMIAL_LEAST_SQUARES_H