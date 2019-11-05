#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

double** vector2array(vector<vector<double>> const& vec) {
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

void transpose(vector<vector<double>> &v)
{
    if (v.size() == 0) {
        return;
    }

    vector<vector<double>> trans_vec(v[0].size(), vector<double>());

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            trans_vec[j].push_back(v[i][j]);
        }
    }

    v = trans_vec;
}

double** tensordot(vector<vector<double>> &a, vector<vector<double>> &b) {
    transpose(a);

    const int m = a.size();
    const int n = a[0].size();
    const int p = b.size();
    const int q = b[0].size();

    double** A = vector2array(a);
    double** B = vector2array(b);

    vector<vector<double>> arr(m, vector<double>(q));

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

void print_matrix(double **A, const int m, const int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<double>> a{{1,2,3,4,5,6,7,8,9}};
    vector<vector<double>> b{{1,2,3,4,5}};
//     vector<vector<double>> a{{-0.000421051,
//         -0.020172,
//         -0.070199,
//         0.198211,
//         0.862476,
//         0.862476,
//         0.198211,
//         -0.070199,
//         -0.020172,
//         -0.000421051}};
//     vector<vector<double>> b{{-0.000421051,
//             -0.0547368,
//             1,
//             -0.0547368,
//             -0.000421051}};

    double** res = tensordot(a, b);

    print_matrix(res, 9, 5);

    return 0;
}