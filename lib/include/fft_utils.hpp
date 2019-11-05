#ifndef FFT_UTILS_HPP
#define FFT_UTILS_HPP

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <string>
#include <complex>
#include <cmath>
#include <iomanip>

/**
 * Header file with a class for operations with 
 * the Fast Fourier Transform.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class FFTUtils {

    public:

        FFTUtils();
        ~FFTUtils();

        cv::Mat fft_shift(cv::Mat const &spectrum);
        cv::Mat fft2(cv::Mat const &img);
        std::vector<std::vector<double>> fft2array(cv::Mat const &spectrum);
};

#endif