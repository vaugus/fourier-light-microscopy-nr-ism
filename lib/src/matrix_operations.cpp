#include "../include/matrix_operations.hpp"

/**
 * Implementation of the MatrixOperations class.
 *
 * @author Victor Augusto
 * @version 1.0
 */


/**
 * Default constructor.
 */
MatrixOperations::MatrixOperations() {

}

/**
 * Default destructor.
 */
MatrixOperations::~MatrixOperations() {

}

/**
 * Prints a vector on the terminal.
 *
 * @param v     Vector of real numbers to be printed.
 */
void MatrixOperations::print_vector(std::vector<double> const& v) {
    for (auto const& i: v) {
        std::cout << i << std::endl;
    }
}

/**
 * Prints the 2D array on the terminal.
 *
 * @param A     Array of real numbers to be printed.
 * @param m     Number of rows of the array.
 * @param n     Number of columns of the array.
 */
void MatrixOperations::print_matrix(double **A, const int m, const int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << A[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

/**
 * Performs the transpose operation on a 2D vector.
 *
 * @param v     Vector to be transposed.
 */
void MatrixOperations::transpose(std::vector<std::vector<double>> &v)
{
    if (v.size() == 0) {
        return;
    }

    std::vector<std::vector<double>> trans_vec(v[0].size(), std::vector<double>());

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            trans_vec[j].push_back(v[i][j]);
        }
    }

    v = trans_vec;
}

/**
 * Transfers the data from the 2D vector to a new 2D array.
 *
 * @param vec   2D vector with the data.
 *
 * @return      2D array with the data from the vector.
 */
double** MatrixOperations::vector2array(std::vector<std::vector<double>> const& vec) {
    const int m = vec.size();
    const int n = vec[0].size();

    double** arr;
    arr = new double*[m];

    for (int i = 0; i < m; i++) {
        arr[i] = new double[n];

        for (int j = 0; j < n; j++) {
            arr[i][j] = 0;
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = vec[i].at(j);
        }
    }

    return arr;
}

/**
 * Generates a vector of vectors, a representation of a matrix from a single
 * one dimensional vector.
 *
 * @param vec   1D vector with the data.
 *
 * @return      2D vector with the data from the 1D vector.
 */
std::vector<std::vector<double>> MatrixOperations::generate_2D_vector(std::vector<double> const& vec) {
    std::vector<std::vector<double>> v{vec};
    return v;
}

/**
 * Performs the dot product of two matrices.
 *
 * @param a     First array of real numbers.
 * @param b     Second array of real numbers.
 * @param m     Number of rows of the first array.
 * @param p     Number of rows of the second array.
 * @param q     Number of columns of the second array.
 *
 * @return      An array with the matrix multiplication result.
 */
double** MatrixOperations::dot(double **a, double **b, int m, int p, int q) {
    std::vector<std::vector<double>> tmp(m, std::vector<double>(q));

    int z = 0;
    double sum = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < q; j++) {
            for (int k = 0; k < p; k++) {
                sum += a[i][k] * b[k][j];
            }

            tmp[i][j] = sum;
            sum = 0;
        }
    }

    return vector2array(tmp);
}

/**
 * Performs the tensor product of two matrices.
 *
 * @param a     First matrix of real numbers.
 * @param b     Second matrix of real numbers.
 *
 * @return      A matrix with the tensor product result
 */
double** MatrixOperations::tensordot(std::vector<std::vector<double>> &a, 
                                     std::vector<std::vector<double>> &b) {
    transpose(a);

    const int m = a.size();
    const int n = a[0].size();
    const int p = b.size();
    const int q = b[0].size();

    double** A = vector2array(a);
    double** B = vector2array(b);

    // resulting matrix creation
    std::vector<std::vector<double>> arr(m, std::vector<double>(q));

    // tensordot product
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < q; j++) {
            for (int k = 0; k < p; k++) {
                arr[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return vector2array(arr);
}

/**
 * Computes the euclidean norm on a real valued matrix.
 *
 * @param arr   The input complex matrix.
 * @param m     The number of rows of the matrix.
 * @param n     The number of columns of the matrix.
 *
 * @return      The real value of the euclidean norm.
 */
double MatrixOperations::euclidean_norm(std::vector<std::vector<double>> const& arr, 
                                        const int m, const int n) {
    double res = 0;
    double val = 0;
    std::vector<double> row;

    for (int i = 0; i < m; i++) {
        row = arr.at(i);

        for (int j = 0; j < n; j++) {
            val = row.at(j);
            res += pow(val, 2);
        }
        row.clear();
    }

    return sqrt(res);
}