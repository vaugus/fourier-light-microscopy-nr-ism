/**
 * @file fft.hpp
 *
 * @brief Header file for class.
 *
 * Header file with a class for n
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#ifndef FFT_HPP
#define FFT_HPP

#include <utility>

#include "ftiqa/sample_function_base.hpp"

/**
 * Header file with a class for operations concerning 
 * the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class SampleFunction : public SampleFunctionBase {

    public:

        void runBatchFFT(std::vector<cv::Mat> &images);
        void sortEnergyDistances(std::vector<std::pair<int, double>> &arr);

    private:

        unsigned getFinalSpectrumSize(cv::Mat const &img);
};

#endif
