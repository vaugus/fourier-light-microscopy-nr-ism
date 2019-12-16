#include "../include/sample_function.hpp"
#include "../include/fft_utils.hpp"
#include "../include/csv.hpp"
#include "../include/constants.hpp"
#include "../include/statistical_analysis.hpp"
#include <ctime>

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
void SampleFunction::run_batch_fft(std::vector<cv::Mat> &img_vec) {
    // Generate the radial vectors for all spectra
    const unsigned n = get_final_spectrum_size(img_vec[0]);
    SampleFunctionBase::generate_radial_vectors(n);

    StatisticalAnalysis *analysis = new StatisticalAnalysis();

    CSVWriter *writer = new CSVWriter();
    writer->set_delimiter(",");
    writer->set_linecount(0);

    cv::Mat gray_spectrum;
    std::vector<std::vector<double>> coefficients;
    std::vector<double> tmp;

    std::vector<cv::Mat>::iterator it;
    for (it = std::begin(img_vec); it != std::end(img_vec); ++it) {
        // perform the fast fourier transform
        SampleFunctionBase::fft(*it, gray_spectrum);

        // extract the coefficient vector
        tmp = SampleFunctionBase::compute_sample_function(gray_spectrum);

        coefficients.emplace_back(tmp);

        gray_spectrum.release();
        tmp.clear();
    }

    std::vector<std::vector<double>> arr = analysis->compute_kurtosis_all_crop_sizes(coefficients);
    std::cout << analysis->find_maximum_range(arr) << std::endl;

    writer->set_filename("output/descriptor/dataset.csv");
    writer->write_fft_descriptor_dataset(coefficients);

    img_vec.clear();
    delete writer;
}


void SampleFunction::sort_energy_distances(std::vector<std::pair<int, double>> &arr) {

    auto comparator = [&](std::pair<int, double> a, std::pair<int, double> b)-> bool {
        return a.second > b.second;
    };

    // sort the maps according to the mean 
    sort(arr.begin(), arr.end(), comparator);
}

unsigned SampleFunction::get_final_spectrum_size(cv::Mat const &img) {
    return std::max(img.rows, img.cols) / 2;
}