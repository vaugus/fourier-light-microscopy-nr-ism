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
 * @param images    The image to suffer the analysis.
 * @param params    The image to suffer the analysis.
 */
void SampleFunction::runBatchFFT(std::vector<cv::Mat> &images) {
    // Generate the radial vectors for all spectra
    const unsigned n = getFinalSpectrumSize(images[0]);
    SampleFunctionBase::generateRadialVectors(n);

    StatisticalAnalysis *analysis = new StatisticalAnalysis();

    CSVWriter *writer = new CSVWriter();
    writer->setDelimiter(",");
    writer->setLineCount(0);

    cv::Mat graySpectrum;
    std::vector<std::vector<double>> coefficients;
    std::vector<double> tmp;

    std::vector<cv::Mat>::iterator it;
    for (it = std::begin(images); it != std::end(images); ++it) {
        // perform the fast fourier transform
        SampleFunctionBase::fft(*it, graySpectrum);

        // extract the coefficient vector
        tmp = SampleFunctionBase::computeSampleFunction(graySpectrum);

        coefficients.emplace_back(tmp);

        graySpectrum.release();
        tmp.clear();
    }

    std::vector<std::vector<double>> data;
    std::vector<std::vector<double>> kurtosisArray;

    analysis->computeKurtosisAllCropSizes(coefficients, data, kurtosisArray);
    
    const unsigned crop = analysis->findMaximumRange(kurtosisArray);
    
    analysis->computeDatasetIqr(data, crop);
    
    writer->setFilename("output/descriptor/dataset.csv");
    writer->writeFFTDescriptorDataset(coefficients);

    images.clear();
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
