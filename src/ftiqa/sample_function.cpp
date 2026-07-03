#include "ftiqa/sample_function.hpp"

#include <algorithm>
#include <ctime>

#include "ftiqa/csv.hpp"
#include "ftiqa/statistical_analysis.hpp"

using namespace std;

/**
 * Implementation of the SampleFunction class.
 *
 * @author Victor Augusto
 * @version 1.0
 */


/**
 * Parametrizes a batch FFT approach execution.
 *
 * @param img_vec      The image to suffer the analysis.
 * @param params    The image to suffer the analysis.
 */
void SampleFunction::runBatchFFT(std::vector<cv::Mat> &img_vec) {
    // Generate the radial vectors for all spectra
    const unsigned n = getFinalSpectrumSize(img_vec[0]);
    SampleFunctionBase::generateRadialVectors(n);

    StatisticalAnalysis *analysis = new StatisticalAnalysis();

    CSVWriter *writer = new CSVWriter();
    writer->setDelimiter(",");
    writer->setLineCount(0);

    cv::Mat gray_spectrum;
    std::vector<std::vector<double>> coefficients;
    std::vector<double> tmp;

    std::vector<cv::Mat>::iterator it;
    for (it = std::begin(img_vec); it != std::end(img_vec); ++it) {
        // perform the fast fourier transform
        SampleFunctionBase::fft(*it, gray_spectrum);

        // extract the coefficient vector
        tmp = SampleFunctionBase::computeSampleFunction(gray_spectrum);

        coefficients.emplace_back(tmp);

        gray_spectrum.release();
        tmp.clear();
    }

    std::vector<std::vector<double>> data;
    std::vector<std::vector<double>> kurtosis_array;

    analysis->computeKurtosisAllCropSizes(coefficients, data, kurtosis_array);
    
    const unsigned crop = analysis->findMaximumRange(kurtosis_array);
    
    analysis->computeDatasetIqr(data, crop);
    
    writer->setFilename("output/descriptor/dataset.csv");
    writer->writeFFTDescriptorDataset(coefficients);

    img_vec.clear();
    delete writer;
}


void SampleFunction::sortEnergyDistances(std::vector<std::pair<int, double>> &arr) {

    auto comparator = [&](std::pair<int, double> a, std::pair<int, double> b)-> bool {
        return a.second > b.second;
    };

    // sort the maps according to the mean 
    sort(arr.begin(), arr.end(), comparator);
}

unsigned SampleFunction::getFinalSpectrumSize(cv::Mat const &img) {
    return std::max(img.rows, img.cols) / 2;
}
