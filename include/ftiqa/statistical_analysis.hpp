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
#include <ftiqa/configuration.hpp>

/**
 * Header file with a class for operations concerning 
 * the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class StatisticalAnalysis {

    public:
        std::vector<double> computeDatasetIqr(std::vector<std::vector<double>>
            const& data,
            const unsigned crop);

        double median(std::vector<double> data);
    
        void computeKurtosisAllCropSizes(std::vector<std::vector<double>> const& dataset,
                                             std::vector<std::vector<double>> &probabilities,
                                             std::vector<std::vector<double>> &kurtosisArray);

        unsigned findMaximumRange(std::vector<std::vector<double>> const& kurtosisArray);

    private:

        double kurtosis(std::vector<double> data);
        double moment(std::vector<double> data, const int r);
        double mean(std::vector<double> const& data);
        double iqr(std::vector<double> data);
};

#endif