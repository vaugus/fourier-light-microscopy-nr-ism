/**
 * @file fourier_sharpness_base.cpp
 *
 * @brief Source file for the implementation of the
 * FourierSharpnessBase class.
 *
 * Source file with the implementation of the 
 * FourierSharpnessBase class for operations 
 * concerning the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#include <omp.h>
#include "../include/fourier_sharpness_base.hpp"
#include "../include/fft_utils.hpp"
#include "../include/csv.hpp"
#include "../include/constants.hpp"

/**
 * Default constructor.
 */
FourierSharpnessBase::FourierSharpnessBase() {
    this->initialize_constants(5, 110);
}

/**
 * Default destructor.
 */    
FourierSharpnessBase::~FourierSharpnessBase() {
}

/*******************************************************************
 *******************************************************************
 * Getter and Setter methods.
 *******************************************************************
 *******************************************************************
 */
void FourierSharpnessBase::set_smallest_vector_size(int smallest_vector_size) {
    this->smallest_vector_size = smallest_vector_size;
}

int FourierSharpnessBase::get_smallest_vector_size() {
    return this->smallest_vector_size;
}


void FourierSharpnessBase::set_center(const unsigned xc, const unsigned yc) {
    this->center = cv::Point(xc, yc);
}

cv::Point FourierSharpnessBase::get_center() {
    return this->center;
}

void FourierSharpnessBase::set_radial_vector_masks(std::vector<cv::Mat> const& radial_vector_masks) {
    this->radial_vector_masks = radial_vector_masks;
}


std::vector<cv::Mat> FourierSharpnessBase::get_radial_vector_masks() {
    return this->radial_vector_masks;
}

void FourierSharpnessBase::set_indices(std::vector<std::vector<cv::Point>> &indices) {
    this->indices = indices;
}


std::vector<std::vector<cv::Point>> FourierSharpnessBase::get_indices() {
    return this->indices;
}

/**
 * Computes all cos and sin values for each of the given angles within the given
 * interval. Also sets the smallest vector size to "infinity". 
 */ 
void FourierSharpnessBase::initialize_constants(int const step, int const limit) {
    set_smallest_vector_size(INT_MAX);

    // set this->cosines with the sequence {0,5,10,...,110}
    for (int angle = 0; angle <= limit; angle += step) {
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
void FourierSharpnessBase::fft(cv::Mat const &image, cv::Mat &gray_spectrum) {
    FFTUtils *fft_utils = new FFTUtils();
    Helper *helper = new Helper();

    // convert to grayscale colourspaces
    cv::Mat gray = helper->luminance(image);

    cv::Size size(image.cols / 2,  image.rows / 2);
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
void FourierSharpnessBase::crop_indices() {
    std::vector<std::vector<cv::Point>> indices = get_indices();
    // crop all the indices until they have the 'lst' size
    for (auto &elem: indices) {
        elem.resize(get_smallest_vector_size());
    }

    set_indices(indices);
}

/**
 * Draws a set of radial vectors to a binary mask,
 * applies the mask and finds the indices of the 
 * nonzero elements. 
 *
 * @param n     The dimension of the square matrix, resultant from
 *              the Fourier Transform.
 */
void FourierSharpnessBase::generate_radial_vectors(const int n) {
    const cv::Mat zeros = cv::Mat::zeros(n, n, CV_8UC1);

    const unsigned radius = unsigned(n / 2);
    
    set_center(radius, radius);

    cv::Point p2;

    // Mat object to hold the indices of non-zero points
    // among the mask.
    cv::Mat idx;

    // Mat object for finding the indices of the non-zero 
    // points among the mask.
    cv::Mat tmp_mask;

    std::vector<cv::Point> tmp_white_points;
    std::vector<std::vector<cv::Point>> tmp_indices;
    std::vector<cv::Mat> tmp_radial_vector_masks;

    for (int i = 0; i < this->cosines.size(); i++) {

        // calculate the end point of the vector, based on the angle
        p2.x = (int)round(center.x + radius * this->cosines[i]);
        p2.y = (int)round(center.y + radius * this->sines[i]);

        tmp_mask = zeros.clone();
        
        // draw the line 
        cv::line(tmp_mask,
                 get_center(),
                 p2,
                 cv::Scalar(255, 255, 255),
                 1,
                 16);

        // get all the indices from the vector
        cv::findNonZero(tmp_mask, idx);
        for (int i = 0; i < idx.rows; i++) {
            const cv::Point* Mi = idx.ptr<cv::Point>(i);
            for(int j = 0; j < idx.cols; j++) {
                tmp_white_points.emplace_back(Mi[j]);
            }
        }

        // get the smallest vector
        if (idx.rows < get_smallest_vector_size()) {
            set_smallest_vector_size(idx.rows);
        }

        tmp_radial_vector_masks.emplace_back(tmp_mask);
        tmp_indices.emplace_back(tmp_white_points);

        tmp_white_points.clear();
        tmp_mask.release();
        idx.release();
    }

    set_radial_vector_masks(tmp_radial_vector_masks);
    set_indices(tmp_indices);
    crop_indices();
}

/**
 * For every radial vector mask, computes the element-wise multiplication with the 
 * given spectrum and inserts it into a vector, which will be turned into the 
 * descriptor in the subsequent stages.  
 */ 
std::vector<cv::Mat> FourierSharpnessBase::apply_radial_vector_masks(cv::Mat const &spectrum) {
    std::vector<cv::Mat> masked_spectra;
    cv::Mat tmp_spectrum;

    for (auto const& mask : get_radial_vector_masks()) {
        spectrum.copyTo(tmp_spectrum, mask);
        masked_spectra.emplace_back(tmp_spectrum);
        tmp_spectrum.release();
    }

    return masked_spectra;
}

/**
 * Performs the element-wise sum of each masked spectrum in order to generate
 * an one-dimensional vector, then divides every element by the count of all
 * vectors. 
 */ 
std::vector<double> FourierSharpnessBase::process_radial_vectors(std::vector<cv::Mat> &masked_spectra) {
    std::vector<double> sum(get_smallest_vector_size());

    // obtain all the masked pixels and sum them
    for (auto const elem : get_indices()) {
        for (auto const& spectra : masked_spectra) {
            for (unsigned i = 0; i < elem.size(); i++) {
                sum[i] += spectra.at<double>(elem[i]);
            }
        }
    }
    
    // divide all elements by the number of vectors taken
    const double k = get_indices().size();
    std::transform(sum.begin(), sum.end(), sum.begin(), 
        [k](double& c) { return c / k; });

    return sum;
} 


/**
 * Retrieves the sum of each element of eight radii 
 * of a spectrum in the form of vector<double>.
 */
std::vector<double> FourierSharpnessBase::compute_descriptor(cv::Mat const &spectrum) {
    std::vector<cv::Mat> masked_spectra = apply_radial_vector_masks(spectrum);
    std::vector<double> sum = process_radial_vectors(masked_spectra);
    return sum;
}