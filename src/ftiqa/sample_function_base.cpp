/**
 * @file fourier_sharpness_base.cpp
 *
 * @brief Source file for the implementation of the
 * SampleFunctionBase class.
 *
 * Source file with the implementation of the 
 * SampleFunctionBase class for operations 
 * concerning the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#include "ftiqa/sample_function_base.hpp"

#include <algorithm>
#include <climits>
#include <cmath>

#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include "ftiqa/fft_utils.hpp"

/**
 * Default constructor.
 */
SampleFunctionBase::SampleFunctionBase() {
    auto max_angle = Configuration::instance().getMaxAngle();
    auto angle_step = Configuration::instance().getAngleStep();
    this->initializeConstants(angle_step, max_angle);
}

/**
 * Default destructor.
 */    
SampleFunctionBase::~SampleFunctionBase() {
}


/**
 * Computes all cos and sin values for each of the given angles within the given
 * interval. Also sets the smallest vector size to "infinity". 
 */ 
void SampleFunctionBase::initializeConstants(int const angle_step, int const max_angle) {
    setSmallestVectorSize(INT_MAX);

    // set this->cosines with the sequence {0,5,10,...,110}
    for (int angle = 0; angle <= max_angle; angle += angle_step) {
        this->cosines.emplace_back(angle);
        this->sines.emplace_back(angle);
    }

    // compute the cosines of each angle and store them in this->cosines
    std::transform(this->cosines.begin(), this->cosines.end(), this->cosines.begin(),
        [](double angle) -> double { return cos(angle * M_PI / 180.0); });

    // compute the sines of each angle and store them in this->sines
    std::transform(this->sines.begin(), this->sines.end(), this->sines.begin(),
        [](double angle) -> double { return sin(angle * M_PI / 180.0); });
}


/**
 * Performs the Discrete Fourier Transform on the given image and
 * stores the result in a Mat object. First, the image is converted
 * to the grayscale colour space and resized to its half. A contrast
 * limited histogram enhancement is done to correct low light images,
 * then the DFT is computed.
 *
 * @param image             The image to be transformed.
 * @param gray_spectrum     A Mat object to store the DFT result.
 */
void SampleFunctionBase::fft(cv::Mat const &image, cv::Mat &gray_spectrum) {
    FFTUtils *fft_utils = new FFTUtils();

    double resize_factor = Configuration::instance().getResizeFactor();

    // convert to grayscale colourspaces
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Size size(image.cols * resize_factor,  image.rows * resize_factor);
    cv::Mat tmp;
    cv::resize(gray, tmp, size);

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    cv::Mat dst;
    clahe->apply(tmp, dst);

    // perform the fft on the converted images
    gray_spectrum = fft_utils->fft2(dst);

    // clear some memory from the read images
    tmp.release();
    dst.release();
    gray.release();

    delete fft_utils;
}

/**
 * Crops each vector of radial vector locations to the smallest
 * vector size.
 */
void SampleFunctionBase::cropIndices() {
    std::vector<std::vector<cv::Point>> indices = getIndices();

    // crop all the indices until they have the 'lst' size
    for (auto &elem: indices) {
        elem.resize(getSmallestVectorSize());
    }

    setIndices(indices);
}

/**
 * Draws a set of radial vectors to a binary mask,
 * applies the mask and finds the indices of the 
 * nonzero elements. 
 *
 * @param n     The dimension of the square matrix, resultant from
 *              the Fourier Transform.
 */
void SampleFunctionBase::generateRadialVectors(const int n) {
    const cv::Mat zeros = cv::Mat::zeros(n, n, CV_8UC1);

    const unsigned radius = unsigned(n / 2);
    
    setCenter(radius, radius);

    cv::Point p2;

    // Mat object to hold the indices of non-zero points
    // among the mask.
    cv::Mat indices;

    // Mat object for finding the indices of the non-zero 
    // points among the mask.
    cv::Mat tmp_mask;    
    
    std::vector<cv::Point> tmp_white_points;
    std::vector<std::vector<cv::Point>> tmp_indices;
    std::vector<cv::Mat> tmp_radial_vector_masks;

    for (std::size_t i = 0; i < this->cosines.size(); i++) {

        // calculate the end point of the vector, based on the angle
        p2.x = (int)round(center.x + radius * this->cosines[i]);
        p2.y = (int)round(center.y + radius * this->sines[i]);

        tmp_mask = zeros.clone();
        
        // draw the line 
        cv::line(tmp_mask,
                 getCenter(),
                 p2,
                 cv::Scalar(255, 255, 255),
                 1,
                 16);

        // get all the indices from the vector
        for (int i = 0; i < tmp_mask.rows; i++) {
            const unsigned char* elem = tmp_mask.ptr<unsigned char>(i);
           
            for (int j = 0; j < tmp_mask.cols; j++) {
                if (elem[j] != 0) {
                    tmp_white_points.emplace_back(cv::Point(i, j));
                }
            }
        }

        // get the smallest vector
        if (tmp_white_points.size() < static_cast<std::size_t>(getSmallestVectorSize())) {
            setSmallestVectorSize(tmp_white_points.size());
        }

        tmp_radial_vector_masks.emplace_back(tmp_mask);
        tmp_indices.emplace_back(tmp_white_points);

        tmp_white_points.clear();
        tmp_mask.release();
    }

    setRadialVectorMasks(tmp_radial_vector_masks);
    setIndices(tmp_indices);
    cropIndices();
}

/**
 * For every radial vector mask, computes the element-wise multiplication with the 
 * given spectrum and inserts it into a vector, which will be turned into the 
 * descriptor in the subsequent stages.  
 */ 
std::vector<cv::Mat> SampleFunctionBase::applyRadialVectorMasks(cv::Mat const &spectrum) {
    std::vector<cv::Mat> masked_spectra;
    cv::Mat tmp;

    std::vector<cv::Mat> masks = getRadialVectorMasks();

    std::vector<cv::Mat>::iterator it;
    for (it = std::begin(masks); it != std::end(masks); ++it) {
        spectrum.copyTo(tmp, *it);
        masked_spectra.emplace_back(tmp);
        tmp.release();
    }

    return masked_spectra;
}

/**
 * Performs the element-wise sum of each masked spectrum in order to generate
 * an one-dimensional vector, then divides every element by the count of all
 * vectors. 
 */ 
std::vector<double> SampleFunctionBase::processRadialVectors(std::vector<cv::Mat> &masked_spectra) {
    std::vector<double> sum(getSmallestVectorSize());

    std::vector<cv::Mat>::iterator masked;
    cv::Mat tmp;

    // obtain all the masked pixels and sum them
    for (auto const& elem : getIndices()) {
        for (masked = masked_spectra.begin(); masked != masked_spectra.end(); ++masked) {
            tmp = *masked;
            
            for (unsigned i = 0; i < elem.size(); i++) {
                sum[i] += tmp.at<double>(elem[i]);
            }
            
            tmp.release();
        }
    }

    // divide all elements by the number of vectors taken
    const double k = getIndices().size();
    std::transform(sum.begin(), sum.end(), sum.begin(), 
        [k](double& c) { return c / k; });

    return sum;
} 


/**
 * Retrieves the sum of each element of eight radii 
 * of a spectrum in the form of vector<double>.
 */
std::vector<double> SampleFunctionBase::computeSampleFunction(cv::Mat const &spectrum) {
    std::vector<cv::Mat> masked_spectra = applyRadialVectorMasks(spectrum);
    std::vector<double> sum = processRadialVectors(masked_spectra);
    return sum;
}


/*******************************************************************
 *******************************************************************
 * Getter and Setter methods.
 *******************************************************************
 *******************************************************************
 */
void SampleFunctionBase::setSmallestVectorSize(int smallest_vector_size) {
    this->smallest_vector_size = smallest_vector_size;
}

int SampleFunctionBase::getSmallestVectorSize() {
    return this->smallest_vector_size;
}


void SampleFunctionBase::setCenter(const unsigned xc, const unsigned yc) {
    this->center = cv::Point(xc, yc);
}

cv::Point SampleFunctionBase::getCenter() {
    return this->center;
}

void SampleFunctionBase::setRadialVectorMasks(std::vector<cv::Mat> const& radial_vector_masks) {
    this->radial_vector_masks = radial_vector_masks;
}


std::vector<cv::Mat> SampleFunctionBase::getRadialVectorMasks() {
    return this->radial_vector_masks;
}

void SampleFunctionBase::setIndices(std::vector<std::vector<cv::Point>> &indices) {
    this->indices = indices;
}


std::vector<std::vector<cv::Point>> SampleFunctionBase::getIndices() {
    return this->indices;
}
