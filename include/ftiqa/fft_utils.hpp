#ifndef FFT_UTILS_HPP
#define FFT_UTILS_HPP

#include <opencv2/core.hpp>

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
};

#endif
