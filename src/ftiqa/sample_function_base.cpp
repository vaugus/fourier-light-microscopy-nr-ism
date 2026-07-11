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

SampleFunctionBase::SampleFunctionBase() {
    auto angleStep = Configuration::instance().getAngleStep();
    auto maxAngle = Configuration::instance().getMaxAngle();
    this->initializeConstants(angleStep, maxAngle);
    setSmallestVectorSize(INT_MAX);
}

SampleFunctionBase::~SampleFunctionBase() {
}

/**
 * Computes all cos and sin values for each of the given angles within the given
 * interval. Also sets the smallest vector size to "infinity". 
 */ 
void SampleFunctionBase::initializeConstants(int const angleStep, int const maxAngle) {

    // set this->cosines with the sequence {0,5,10,...,110}
    for (int angle = 0; angle <= maxAngle; angle += angleStep) {
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
 * @param graySpectrum     A Mat object to store the DFT result.
 */
void SampleFunctionBase::fft(cv::Mat image, cv::Mat &graySpectrum) {
    auto fftUtils = std::make_unique<FFTUtils>();

    double resizeFactor = Configuration::instance().getResizeFactor();

    // convert to grayscale colourspaces
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Size size(image.cols * resizeFactor,  image.rows * resizeFactor);
    cv::resize(image, image, size);

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->apply(image, image);

    // perform the fft on the converted image
    graySpectrum = fftUtils->fft2(image);
}

/**
 * Crops each vector of radial vector locations to the smallest
 * vector size.
 */
void SampleFunctionBase::cropIndices() {
    std::vector<std::vector<cv::Point>> indices = getIndices();

    // crop all the indices until they have the 'smallest' size
    for (auto &index: indices) {
        index.resize(getSmallestVectorSize());
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
    const unsigned radius = unsigned(n / 2);
    
    setCenter(radius, radius);

    cv::Point head;

    std::vector<std::vector<cv::Point>> indices;
    std::vector<cv::Mat> radialVectorMasks;
    
    for (std::size_t i = 0; i < this->cosines.size(); i++) {
        std::vector<cv::Point> whitePoints;

        // calculate the end point of the vector, based on the angle
        head.x = (int)round(center.x + radius * this->cosines[i]);
        head.y = (int)round(center.y + radius * this->sines[i]);

        // draw the line
        cv::Mat line = cv::Mat::zeros(n, n, CV_8UC1);
        cv::line(line,
                 getCenter(),
                 head,
                 cv::Scalar(255, 255, 255),
                 1,
                 16);

        // get all the indices from the vector
        for (int i = 0; i < line.rows; i++) {
            const unsigned char* elem = line.ptr<unsigned char>(i);
           
            for (int j = 0; j < line.cols; j++) {
                if (elem[j] != 0) {
                    whitePoints.emplace_back(cv::Point(i, j));
                }
            }
        }

        // get the smallest vector
        if (whitePoints.size() < static_cast<std::size_t>(getSmallestVectorSize())) {
            setSmallestVectorSize(whitePoints.size());
        }

        radialVectorMasks.emplace_back(line);
        indices.emplace_back(whitePoints);
    }

    setRadialVectorMasks(radialVectorMasks);
    setIndices(indices);
    cropIndices();
}

/**
 * For every radial vector mask, computes the element-wise multiplication with the 
 * given spectrum and inserts it into a vector, which will be turned into the 
 * descriptor in the subsequent stages.  
 */ 
std::vector<cv::Mat> SampleFunctionBase::applyRadialVectorMasks(cv::Mat const &spectrum) {
    std::vector<cv::Mat> maskedSpectra;
    std::vector<cv::Mat> masks = getRadialVectorMasks();

    for (auto const& mask : masks) {
        cv::Mat maskedSpectrum;
        spectrum.copyTo(maskedSpectrum, mask);
        maskedSpectra.emplace_back(std::move(maskedSpectrum));
    }

    return maskedSpectra;
}

/**
 * Performs the element-wise sum of each masked spectrum in order to generate
 * an one-dimensional vector, then divides every element by the count of all
 * vectors. 
 */ 
std::vector<double> SampleFunctionBase::processRadialVectors(std::vector<cv::Mat> &maskedSpectra) {
    std::vector<double> sum(getSmallestVectorSize());

    // obtain all the masked pixels and sum them
    for (auto const& index : getIndices()) {
        for (auto maskedSpectrum : maskedSpectra) {
            for (unsigned i = 0; i < index.size(); i++) {
                sum[i] += maskedSpectrum.at<double>(index[i]);
            }
        }
    }

    // divide all elements by the number of vectors taken
    const double numberOfVectors = getIndices().size();
    std::transform(sum.begin(), sum.end(), sum.begin(), 
        [numberOfVectors](double& element) { return element / numberOfVectors; });

    return sum;
} 


/**
 * Retrieves the sum of each element of eight radii 
 * of a spectrum in the form of vector<double>.
 */
std::vector<double> SampleFunctionBase::computeSampleFunction(cv::Mat const &spectrum) {
    std::vector<cv::Mat> maskedSpectra = applyRadialVectorMasks(spectrum);
    std::vector<double> sum = processRadialVectors(maskedSpectra);
    return sum;
}


void SampleFunctionBase::setSmallestVectorSize(int size) {
    smallestVectorSize = size;
}

int SampleFunctionBase::getSmallestVectorSize() {
    return smallestVectorSize;
}


void SampleFunctionBase::setCenter(const unsigned xc, const unsigned yc) {
    this->center = cv::Point(xc, yc);
}

cv::Point SampleFunctionBase::getCenter() {
    return this->center;
}

void SampleFunctionBase::setRadialVectorMasks(std::vector<cv::Mat> const& masks) {
    radialVectorMasks = masks;
}


std::vector<cv::Mat> SampleFunctionBase::getRadialVectorMasks() {
    return radialVectorMasks;
}

void SampleFunctionBase::setIndices(std::vector<std::vector<cv::Point>> &indices) {
    this->indices = indices;
}


std::vector<std::vector<cv::Point>> SampleFunctionBase::getIndices() {
    return indices;
}
