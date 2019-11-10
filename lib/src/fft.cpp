#include "../include/fft.hpp"
#include "../include/fft_utils.hpp"
#include "../include/csv.hpp"
#include "../include/matrix_operations.hpp"
#include "../include/constants.hpp"
#include <ctime>

using namespace std;

/**
 * Implementation of the FFT class.
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
void FFT::run_batch_fft(std::vector<cv::Mat> &img_vec) {
    // Generate the radial vectors for all spectra
    const unsigned n = get_final_spectrum_size(img_vec[0]);
    FourierSharpnessBase::generate_radial_vectors(n);

    CSVWriter *writer = new CSVWriter();
    writer->set_delimiter(",");
    writer->set_linecount(0);

    cv::Mat gray_spectrum;
    std::vector<std::vector<double>> coefficients;
    std::vector<double> tmp;

    for (auto const& rgba : img_vec) {
        // perform the fast fourier transform
        FourierSharpnessBase::fft(rgba, gray_spectrum);

        // extract the coefficient vector
        tmp = FourierSharpnessBase::compute_descriptor(gray_spectrum);

        coefficients.emplace_back(tmp);

        gray_spectrum.release();
        tmp.clear();
    }

    writer->set_filename("output/descriptor/dataset.csv");
    writer->write_fft_descriptor_dataset(coefficients);

    img_vec.clear();
    delete writer;
}


void FFT::sort_energy_distances(std::vector<std::pair<int, double>> &arr) {

    auto comparator = [&](std::pair<int, double> a, std::pair<int, double> b)-> bool {
        return a.second > b.second;
    };

    // sort the maps according to the mean 
    sort(arr.begin(), arr.end(), comparator);
}

unsigned FFT::get_final_spectrum_size(cv::Mat const &img) {
    return std::max(img.rows, img.cols) / 2;
}