#include <vector>
#include <iostream>
#include <cmath>

/**
 * Header file with a class for operations with
 * matrices, 2D arrays and 2D vectors.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class MatrixOperations {

    public:

        MatrixOperations();
        ~MatrixOperations();

        void print_vector(std::vector<double> const& v);
        void print_matrix(double **a, const int m, const int n);

        void transpose(std::vector<std::vector<double>> &v);
        double** vector2array(std::vector<std::vector<double>> const& vec);
        std::vector<std::vector<double>> generate_2D_vector(std::vector<double> const& vec);

        double** dot(double **a, double **b, const int m,
                    const int p, const int q);

        double** tensordot(std::vector<std::vector<double>> &a, std::vector<std::vector<double>> &b);

        double euclidean_norm(std::vector<std::vector<double>> const& arr, const int m, const int n);
};