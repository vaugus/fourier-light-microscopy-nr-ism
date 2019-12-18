/**
 * @file statistical_analysis.hpp
 *
 * @brief Header file for class StatisticalAnalysis.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include <vector>

/**
 * Header file with a class for operations concerning 
 * the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class StatisticalAnalysis {

    public:
        std::vector<double> compute_dataset_iqr(std::vector<std::vector<double>>
            const & kurtosis_array,
            const unsigned crop);

            double iqr(std::vector<double> data);
            double median(std::vector<double> data);

    private:
    
        std::vector<std::vector<double>> compute_kurtosis_all_crop_sizes(
            std::vector<std::vector<double>> const& dataset);

        unsigned find_maximum_range(std::vector<std::vector<double>> const& kurtosis_array);

        double kurtosis(std::vector<double> data);
        double moment(std::vector<double> data, const int r);
        double mean(std::vector<double> const& data);

        double _mul(double* a, double* b);
        double* _sum(double* a, double* b);
        double* _sub(double* a, double* b);
};

#endif