#include <iostream>
#include <string>
#include <complex>
#include <cmath>
#include <iomanip>
#include "../include/fft_utils.hpp"
#include "../include/constants.hpp"

/**
 * Implementation of the FFTUtils class.
 *
 * @author Victor Augusto
 * @version 1.0
 */

/**
 * Default constructor.
 */
FFTUtils::FFTUtils() {

}


/**
 * Default destructor.
 */
FFTUtils::~FFTUtils() {

}


/**
 * Shifts the zero-frequency component to the center of the spectrum.
 *
 * @param spectrum      The spectrum to be shifted.
 *
 * @return              The shifted spectrum.
 */
cv::Mat FFTUtils::fft_shift(cv::Mat const &spectrum) {
    const unsigned cx = spectrum.cols / 2;
    const unsigned cy = spectrum.rows / 2;

    // Top-Left - Create a ROI per quadrant
    cv::Mat q0(spectrum, cv::Rect(0, 0, cx, cy));

    // Top-Right
    cv::Mat q1(spectrum, cv::Rect(cx, 0, cx, cy));

    // Bottom-Left
    cv::Mat q2(spectrum, cv::Rect(0, cy, cx, cy));

    // Bottom-Right
    cv::Mat q3(spectrum, cv::Rect(cx, cy, cx, cy));

    cv::Mat tmp;

    // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // swap quadrant (Top-Right with Bottom-Left)
    q1.copyTo(tmp);                    
    q2.copyTo(q1);
    tmp.copyTo(q2);

    /** Transform the matrix with double values into a
     * viewable image form (double between values 0 and 1).
     */ 
    normalize(spectrum, spectrum, 0, 1, cv::NORM_MINMAX);
    
    return spectrum;
}


/**
 * Computes the 2-dimensional Discrete Fourier Transform.
 * 
 * @param I     The input image.
 * 
 * @return      The corresponding spectrum.
 */
cv::Mat FFTUtils::fft2(cv::Mat const &img) {
    const unsigned m = img.rows;
    const unsigned n = img.cols;

    const unsigned pad_size = std::max(m, n);
    cv::Mat padded;

    cv::copyMakeBorder(img,
                       padded,
                       0,
                       pad_size - img.rows,
                       0,
                       pad_size - img.cols,
                       cv::BORDER_CONSTANT,
                       cv::Scalar::all(0));

    cv::Mat planes[] = {cv::Mat_<double>(padded), cv::Mat::zeros(padded.size(), CV_64F)};
    cv::Mat complexI;

    // Add to the expanded another plane with zeros
    // this way the result may fit in the source matrix
    cv::merge(planes, 2, complexI);

    // perform dft with the fft algorithm        
    cv::dft(complexI, complexI);

    // split the complex coefficients into two matrices
    cv::split(complexI, planes);

    // compute the magnitude of each complex coefficient
    cv::Mat res;
    cv::magnitude(planes[0], planes[1], planes[0]);
    res = planes[0];

    // switch to logarithmic scale
    res += cv::Scalar::all(1);
    cv::log(res, res);

    // crop the spectrum, if it has an odd number of rows or columns
    // res = res(Rect(0, 0, res.cols & -2, res.rows & -2));

    padded.release();
    planes[0].release();
    planes[1].release();
    complexI.release();

    // perform the shift of the smallest coefficient to the center
    res = fft_shift(res);
    return res;
}