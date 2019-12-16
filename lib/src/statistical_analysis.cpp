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
            cout << crop << endl;
            continue;
        }

        ptp = max - min;
        // cout << std::setprecision(14) << ptp << endl;


        if (ptp > maximum) {
            maximum = ptp;
            ans = crop;
        }
    }

    return ans;
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