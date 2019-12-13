#include "../include/statistical_analysis.hpp"
#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <chrono>
#include <unistd.h>

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
void StatisticalAnalysis::compute_kurtosis_all_crop_sizes(std::vector<std::vector<double>> const& dataset) {
    using namespace std;

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


    // Assemble all possible crop sizes onto a vector
    std::vector<unsigned> crop_sizes(probabilities[0].size());
    std::iota(crop_sizes.begin(), crop_sizes.end(), 0);

    std::vector<unsigned>::iterator crop;

    for (crop = std::begin(crop_sizes); crop != std::end(crop_sizes); ++crop) {
        for (auto prob : probabilities) {
            cout << kurtosis(prob) << endl;
        }
    }

    exit(0);
    // for crop in range(max_length):
    //     # apply the current crop size
    //     kurtosis_arr[crop] = list(map(lambda x: kurtosis(data[x, crop:]), iterable))
        
    // return data, kurtosis_arr, max_length, file_count
}


double StatisticalAnalysis::kurtosis(std::vector<double> data) {
    return moment(data, 4) / std::pow(moment(data, 2), 2);
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