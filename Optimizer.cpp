#include "Optimizer.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <random>

// Random number generator
double random_double(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(min, max);
    return dist(gen);
}

// Constructors
Optimizer::Optimizer() : objectiveFunction(nullptr) {}
Optimizer::Optimizer(func objectiveFunction) : objectiveFunction(objectiveFunction) {}

// Approximation of the gradient by finite differences
std::vector<double> compute_gradient(std::vector<double> x, func objectiveFunction, double h = 1e-5) {
    int dim = x.size();
    std::vector<double> grad(dim);

    for (int i = 0; i < dim; ++i) {
        std::vector<double> x_forward = x, x_backward = x;
        x_forward[i] += h;
        x_backward[i] -= h;
        grad[i] = (objectiveFunction(x_forward) - objectiveFunction(x_backward)) / (2 * h);
    }
    return grad;
}

// Matrix-vector multiplication
std::vector<double> mat_vec_mult(const std::vector<std::vector<double>>& mat, const std::vector<double>& vec) {
    int n = mat.size();
    std::vector<double> result(n, 0.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += mat[i][j] * vec[j];
        }
    }
    return result;
}

// Update of matrix H (DFP)
std::vector<std::vector<double>> update_H(const std::vector<std::vector<double>>& H,
    const std::vector<double>& delta_x,
    const std::vector<double>& delta_grad) {
    int n = delta_x.size();

    double delta_x_dot_delta_grad = std::inner_product(delta_x.begin(), delta_x.end(), delta_grad.begin(), 0.0);
    if (std::abs(delta_x_dot_delta_grad) < 1e-8) {
        return H; // Avoid division by zero, do not update H
    }

    std::vector<double> H_delta_grad = mat_vec_mult(H, delta_grad);
    double delta_grad_dot_H_delta_grad = std::inner_product(delta_grad.begin(), delta_grad.end(), H_delta_grad.begin(), 0.0);

    if (std::abs(delta_grad_dot_H_delta_grad) < 1e-8) {
        return H;
    }

    std::vector<std::vector<double>> new_H = H;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            new_H[i][j] += (delta_x[i] * delta_x[j] / delta_x_dot_delta_grad)
                - (H_delta_grad[i] * H_delta_grad[j] / delta_grad_dot_H_delta_grad);
        }
    }

    return new_H;
}

// Line Search
double Optimizer::line_search(const std::vector<double> x, const std::vector<double>& d) {
    double alpha = 1.0;
    double rho = 0.5;  // Faster reduction 
    double c = 1e-4;

    double f0 = objectiveFunction(x);
    std::vector<double> grad = compute_gradient(x, objectiveFunction);

    while (true) {
        std::vector<double> x_new(x.size());
        for (size_t i = 0; i < x.size(); ++i) {
            x_new[i] = x[i] + alpha * d[i];
        }

        double f_new = objectiveFunction(x_new);
        double lhs = f_new;
        double rhs = f0 + c * alpha * std::inner_product(grad.begin(), grad.end(), d.begin(), 0.0);

        if (lhs <= rhs) {
            break;
        }

        alpha *= rho;
    }

    return alpha;
}

// DFP Algorithm
std::vector<double> Optimizer::dfp(int dim, int max_iter, double tol, double lower_bound, double upper_bound) {
    if (!objectiveFunction) {
        std::cerr << "Error: Objective function not set!" << std::endl;
        return {};
    }

    std::vector<double> x(dim);
    for (int i = 0; i < dim; ++i) {
        x[i] = lower_bound + (upper_bound - lower_bound) * 0.5 + random_double(-5.0, 5.0);
    }

    std::vector<std::vector<double>> H(dim, std::vector<double>(dim, 0.0));
    for (int i = 0; i < dim; ++i) {
        H[i][i] = 1.0;
    }

    std::vector<double> grad = compute_gradient(x, objectiveFunction);
    int iter = 0;

    while (iter < max_iter) {
        double grad_norm = std::sqrt(std::inner_product(grad.begin(), grad.end(), grad.begin(), 0.0));
        if (grad_norm < tol) {
            break;
        }

        std::vector<double> d = mat_vec_mult(H, grad);
        for (double& di : d) di = -di;

        double alpha = line_search(x, d);

        std::vector<double> x_new(dim);
        for (int i = 0; i < dim; ++i) {
            x_new[i] = x[i] + alpha * d[i];
            x_new[i] = std::max(lower_bound, std::min(upper_bound, x_new[i]));
        }

        std::vector<double> grad_new = compute_gradient(x_new, objectiveFunction);
        std::vector<double> delta_x(dim), delta_grad(dim);
        for (int i = 0; i < dim; ++i) {
            delta_x[i] = x_new[i] - x[i];
            delta_grad[i] = grad_new[i] - grad[i];
        }

        double delta_x_dot_delta_grad = std::inner_product(delta_x.begin(), delta_x.end(), delta_grad.begin(), 0.0);
        if (std::abs(delta_x_dot_delta_grad) > 1e-8) {
            H = update_H(H, delta_x, delta_grad);
        }

        x = x_new;
        grad = grad_new;

        std::cout << "Iteration " << iter + 1 << ": Objective value = " << objectiveFunction(x) << std::endl;
        iter++;
    }

    std::cout << "\nOptimization complete after " << iter << " iterations." << std::endl;
    std::cout << "Minimum found at: ";
    for (double xi : x) std::cout << xi << " ";
    std::cout << "\nMinimum value: " << objectiveFunction(x) << std::endl;

    return x;
}
