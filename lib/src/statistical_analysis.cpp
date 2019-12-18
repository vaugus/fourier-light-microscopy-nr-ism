#include "../include/statistical_analysis.hpp"
#include <iostream>
#include <cmath>
#include <map>
#include <omp.h>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cfloat>


/**
 * Implementation of the StatisticalAnalysis class.
 *
 * @author Victor Augusto
 * @version 1.0
 */

using namespace std;

/**
 * Parametrizes a batch FFT approach execution.
 *
 * @param img_vec      The image to suffer the analysis.
 * @param params    The image to suffer the analysis.
 */
std::vector<std::vector<double>> StatisticalAnalysis::compute_kurtosis_all_crop_sizes(std::vector<std::vector<double>> const& dataset) {
    std::vector<std::vector<double>> probabilities;
    std::vector<std::vector<double>> kurtosis_array;

    std::copy(dataset.begin(), dataset.end(), back_inserter(probabilities)); 

    // Apply the operator to transform the data to probabilities
    for (auto & elem : probabilities) {
        auto sum = std::accumulate(elem.begin(), elem.end(), 0.0);
     
        std::transform(elem.begin(), elem.end(), elem.begin(),
            [&](double frequency) -> double { 
                return frequency / sum;
        });
    }

    const unsigned crop_sizes = probabilities[0].size();

    std::vector<double> tmp_probability;
    std::vector<double> tmp_kurtosis;
    std::vector<unsigned>::iterator crop;

    for (unsigned crop = 0; crop < crop_sizes; crop++) {
        for (auto prob: probabilities) {
            tmp_probability = std::vector<double>(prob.begin() + crop, prob.end());
            tmp_kurtosis.emplace_back(kurtosis(tmp_probability));
        }

        kurtosis_array.emplace_back(tmp_kurtosis);
        tmp_kurtosis.clear();
    }

    return kurtosis_array;
}

unsigned StatisticalAnalysis::find_maximum_range(std::vector<std::vector<double>> const& kurtosis_array) {
    unsigned ans = 0;
    double maximum = DBL_MIN;

    const unsigned size = kurtosis_array.size();

    std::vector<double> row;
    double max = 0;
    double min = 0;
    double ptp = 0;

    for (unsigned crop = 0; crop < size; crop++) {
        row = kurtosis_array.at(crop);

        max = *std::max_element(row.begin(), row.end());
        min = *std::min_element(row.begin(), row.end());

        // if the kurtosis resulted in negative values,
        // the crop size is discarded
        if ((max < 0) || (min < 0)) {
            continue;
        }

        ptp = max - min;

        if (ptp > maximum) {
            maximum = ptp;
            ans = crop;
        }
    }

    return ans;
}

std::vector<double> StatisticalAnalysis::compute_dataset_iqr(std::vector<std::vector<double>>
    const & kurtosis_array,
    const unsigned crop) {

    std::vector<double> iqr_array;

    // Index of median of entire data 
    // int mid_index = median(a, 0, n); 
  
    // // Median of first half 
    // int Q1 = a[median(a, 0, mid_index)]; 
  
    // // Median of second half 
    // int Q3 = a[median(a, mid_index + 1, n)]; 

    return iqr_array;
}

double StatisticalAnalysis::iqr(std::vector<double> data) {
    // number of observations
    const unsigned n = data.size();

    double ans = 0;

    // lambda expression to find the quartiles' positions
    auto quar_position = [](unsigned n, unsigned q) {
        if (q == 1) {
            return (n + 3) / 4.0; 
        }

        if (q == 3) {
            return (3.0 * n + 1) / 4.0; 
        }
    };

    auto print = [] (double* arr) {
        for (int i = 0; i < 3; i++) {
            cout << arr[i] << endl;
        }
        cout << endl;
    };

    // computation of the positions of each quartile
    double positions[3] = {quar_position(n, 1), median(data), quar_position(n, 3)};

    print(positions);

    // retrieve upper indices to the left-closest positions to each quartile position
    double indices[3];
    for (unsigned i = 0; i < 3; i++) {
        indices[i] = std::floor(positions[i]); 
    }

    print(indices);

    // retrieve the difference between quartile positions and the floor-rounded indices
    double differences[3] = {positions[0] - indices[0],
                             positions[1] - indices[1],
                             positions[2] - indices[2]};

    print(differences);

    const double Q1 = data[indices[0] - 1] + (data[indices[0]] - data[indices[0] - 1]) * differences[0];
    const double Q3 = data[indices[2] - 1] + (data[indices[2]] - data[indices[2] - 1]) * differences[2];

    cout << Q1 << endl;
    cout << Q3 << endl;

    return Q3 - Q1;
}

double StatisticalAnalysis::median(std::vector<double> data) {
    // number of observations
    const unsigned n = data.size();

    // sort the data in ascending order
    std::sort(data.begin(), data.end());

    double median = 0;

    const unsigned middle = n / 2;

    if (n % 2 != 0) {
        median = data[middle];
    } else {
        median = (data[middle] + data[middle - 1]) / 2.0;
    }

    return median;
}


double StatisticalAnalysis::kurtosis(std::vector<double> data) {
    return (moment(data, 4) / std::pow(moment(data, 2), 2)) - 3.0;
}

double StatisticalAnalysis::moment(std::vector<double> data, const int r) {
    double avg = mean(data);

    double mr = 0.0;
    std::vector<double>::iterator it;
    
    for (it = data.begin(); it != data.end(); ++it) {
        mr += std::pow(*it - avg, r);
    }

    return mr / data.size();
}

double StatisticalAnalysis::mean(std::vector<double> const& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}


double StatisticalAnalysis::_mul(double* a, double* b) {
    const unsigned n = sizeof(a) / sizeof(double);
    double ans = 0.0;

    for (int i = 0; i < n; i++) {
        ans += a[i] * b[i];
    }

    return ans;
}

double* StatisticalAnalysis::_sum(double* a, double* b) {
    const unsigned n = sizeof(a) / sizeof(double);
    double ans[n] = {0};

    for (int i = 0; i < n; i++) {
        ans[i] = a[i] + b[i];
    }

    return ans;
}

double* StatisticalAnalysis::_sub(double* a, double* b) {
    const unsigned n = sizeof(a) / sizeof(double);
    double ans[n] = {0};

    for (int i = 0; i < n; i++) {
        ans[i] = a[i] - b[i];
    }

    return ans;
}