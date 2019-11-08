#include <omp.h>
#include "../include/fourier_sharpness_base.hpp"
#include "../include/fft_utils.hpp"
#include "../include/csv.hpp"
#include "../include/matrix_operations.hpp"
#include "../include/constants.hpp"

/**
 * Implementation of the FourierSharpnessBase class.
 *
 * @author Victor Augusto
 * @version 1.0
 */

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

void FourierSharpnessBase::set_indices(std::vector<std::vector<cv::Point>> const& indices) {
    this->indices = indices;
}


std::vector<std::vector<cv::Point>> FourierSharpnessBase::get_indices() {
    return this->indices;
}


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
 * Pads the image with the specified value.
 *
 * @param rgba            The image to be transformed.
 * @param gray_spectrum     The padding value.
 * @param hsv_spectrum      The padding value.
 * @param ring_masks        The padding value.
 *
 * @return          The padded image.
 */
void FourierSharpnessBase::fft(cv::Mat const &rgba, cv::Mat &gray_spectrum) {
    FFTUtils *fft_utils = new FFTUtils();
    Helper *helper = new Helper();

    // convert to grayscale colourspaces
    cv::Mat gray = helper->luminance(rgba);

    cv::Size size(rgba.cols / 2,  rgba.rows / 2);
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
 * Calculates the energy of each band for the spectrum.
 *
 * @param spectrum      The spectrum to have the band energies calculated.
 * @param ring_masks    The set of ring masks for each band..
 *
 * @return              A vector with the energy values for each band.
 */
std::vector<double> FourierSharpnessBase::band_energy(cv::Mat const& spectrum, 
                                       std::vector<cv::Mat> const& ring_masks) {
    std::vector<double> energies;

    MatrixOperations *mat_ops = new MatrixOperations();
    FFTUtils *fft_utils = new FFTUtils();

    const unsigned m = spectrum.rows;
    const unsigned n = spectrum.cols;

    double energy = 0;
    cv::Mat tmp;
    std::vector<std::vector<double>> fft_array;

    for (auto const& mask : ring_masks) {
        spectrum.copyTo(tmp, mask);

        fft_array = fft_utils->fft2array(tmp);
        energy = mat_ops->euclidean_norm(fft_array, m, n);

        energies.push_back(energy);

        tmp.release();
        energy = 0;
    }

    delete fft_utils;
    delete mat_ops;

    return energies;
}


/**
 * Generates a ring mask corresponding to the frequency band chosen.
 *
 * @param x                 The number of rows of the mask.
 * @param y                 The number of columns of the mask.
 * @param frequency_band    The frequency band to generate the mask.
 *
 * @return                  A Mat mask object.
 */
const cv::Mat FourierSharpnessBase::generate_ring_mask(const int n, std::string const& frequency_band) {
    const unsigned xc = unsigned(n / 2);
    const unsigned yc = unsigned(n / 2);

    const unsigned bw = unsigned(n / 5);
    const unsigned half_bw = unsigned(bw / 2);

    // Create black empty images
    cv::Mat mask = cv::Mat::zeros(n, n, CV_8UC1);

    double radius = 0;

    if (frequency_band.compare(Constants::FREQ_BAND_MID) == 0) {
        radius = yc - (3 * half_bw);
    }

    if (frequency_band.compare(Constants::FREQ_BAND_HIGH) == 0) {
        radius = yc - (2 * half_bw);
    }

    if (frequency_band.compare(Constants::FREQ_BAND_HIGHEST) == 0) {
        radius = yc - (half_bw / 2);
    }
   
    // Draw a circle 
    cv::circle(mask, cv::Point(xc, yc), radius, cv::Scalar(255, 255, 255), half_bw);
    return mask;
}


/**
 * Draws a set of radial vectors to a binary mask,
 * applies the mask and finds the indices of the 
 * nonzero elements. 
 *
 * @param masked_spectra    The FFT spectrum to be analysed.
 * @param indices           The FFT spectrum to be analysed.
 * @param spectrum          The FFT spectrum to be analysed.
 * @param lst               The FFT spectrum to be analysed.
 * @param xc                The FFT spectrum to be analysed.
 * @param yc                The FFT spectrum to be analysed.
 * @param n                 The FFT spectrum to be analysed.
 *
 * @return                  A vector representing the coefficient
 *                          cumulative sum array.
 */
void FourierSharpnessBase::generate_radial_vectors(const int n) {

    const unsigned radius = unsigned(n / 2);
    
    set_center(unsigned(n / 2), unsigned(n / 2));

    cv::Point p2;

    // Mat object to hold the indices of non-zero points
    // among the mask.
    cv::Mat idx;

    // Mat object where the mask will be applied on one
    // of the iterations.
    // cv::Mat tmp;

    // Mat object for finding the indices of the non-zero 
    // points among the mask.
    cv::Mat tmp_mask;

    std::vector<cv::Point> tmp_white_points;
    std::vector<std::vector<cv::Point>> tmp_indices;
    std::vector<cv::Mat> tmp_radial_vector_masks;

    std::cout << this->cosines.size() << std::endl;

    for (int i = 0; i < this->cosines.size(); i++) {

        // calculate the end point of the vector, based on the angle
        p2.x = (int)round(center.x + radius * this->cosines[i]);
        p2.y = (int)round(center.y + radius * this->sines[i]);

        tmp_mask = cv::Mat::zeros(n, n, CV_8UC1);
        
        // draw the line 
        cv::line(tmp_mask,
                 get_center(),
                 p2,
                 cv::Scalar(255, 255, 255),
                 1,
                 16);

        // get all the indices from the vector
        cv::findNonZero(tmp_mask, idx);
        for (unsigned j = 0; j < idx.total(); j++) {
            cv::Point non_zero(idx.at<cv::Point>(j).x, idx.at<cv::Point>(j).y);
            tmp_white_points.emplace_back(non_zero);
        }

        // get the smallest vector
        if (idx.rows < get_smallest_vector_size()) {
            set_smallest_vector_size(idx.rows);
        }

        tmp_radial_vector_masks.emplace_back(tmp_mask);
        tmp_indices.emplace_back(tmp_white_points);

        tmp_indices.clear();
        tmp_mask.release();
        idx.release();
    }

    set_radial_vector_masks(tmp_radial_vector_masks);
    set_indices(indices);
}


/**
 * Draws a set of radial vectors to a binary mask,
 * applies the mask and finds the indices of the 
 * nonzero elements. 
 *
 * @param masked_spectra    The FFT spectrum to be analysed.
 * @param indices           The FFT spectrum to be analysed.
 * @param spectrum          The FFT spectrum to be analysed.
 * @param lst               The FFT spectrum to be analysed.
 * @param xc                The FFT spectrum to be analysed.
 * @param yc                The FFT spectrum to be analysed.
 * @param n                 The FFT spectrum to be analysed.
 *
 * @return                  A vector representing the coefficient
 *                          cumulative sum array.
 */
void FourierSharpnessBase::draw_radial_vectors(std::vector<cv::Mat> &masked_spectra,
                                               std::vector<std::vector<cv::Point>> &indices,
                                               cv::Mat const &spectrum,
                                               int &lst,
                                               const int xc,
                                               const int yc,
                                               const int n) {

    const unsigned radius = unsigned(n / 2);
    
    const cv::Point center(xc, yc);

    cv::Point p2;

    // Mat object to hold the indices of non-zero points
    // among the mask.
    cv::Mat idx;

    // Mat object where the mask will be applied on one
    // of the iterations.
    cv::Mat tmp;

    // Mat object for finding the indices of the non-zero 
    // points among the mask.
    cv::Mat tmp_mask;

    std::vector<cv::Point> vec;

    for (int i = 0; i < this->cosines.size(); i++) {

        // calculate the end point of the vector, based on the angle
        p2.x = (int)round(center.x + radius * this->cosines[i]);
        p2.y = (int)round(center.y + radius * this->sines[i]);

        tmp_mask = cv::Mat::zeros(n, n, CV_8UC1);
        
        // draw the line 
        cv::line(tmp_mask,
                 center,
                 p2,
                 cv::Scalar(255, 255, 255),
                 1,
                 16);

        // apply the mask
        spectrum.copyTo(tmp, tmp_mask);

        // get all the indices from the vector
        cv::findNonZero(tmp_mask, idx);
        for (unsigned j = 0; j < idx.total(); j++) {
            cv::Point p(idx.at<cv::Point>(j).x, idx.at<cv::Point>(j).y);
            vec.emplace_back(p);
        }

        // get the smallest vector
        if (idx.rows < lst) {
            lst = idx.rows;
        }

        masked_spectra.emplace_back(tmp);
        indices.emplace_back(vec);

        vec.clear();
        tmp.release();
        tmp_mask.release();
        idx.release();
    }
}


/**
 * Pads the image with the specified value.
 *
 * @param masked_spectra            The image to be transformed.
 * @param indices     The padding value.
 * @param lst      The padding value.
 *
 * @return          The padded image.
 */
std::vector<double> FourierSharpnessBase::process_radial_vectors(
                                    std::vector<cv::Mat> const &masked_spectra,
                                    std::vector<std::vector<cv::Point>> &indices,
                                    unsigned lst) {


    // 1363
    // crop all the indices until they have the 'lst' size
    for (auto &elem: indices) {
        while (elem.size() > lst) {
            elem.pop_back();
        }
    }

    // obtain all the masked pixels and sum them
    std::vector<double> sum(lst);

    for (auto &elem : indices) {
        for (auto &spectra : masked_spectra) {
            for (unsigned i = 0; i < elem.size(); i++) {
                sum[i] += spectra.at<double>(elem[i]);
            }
        }
    }

    // divide all elements by the number of vectors taken
    const double k = indices.size();
    std::transform(sum.begin(), sum.end(), sum.begin(), 
        [k](double& c) { return c / k; });

    indices.clear();
    return sum;
}


/**
 * Assembles a vector with the ring masks for each band.
 *
 * @param m     The number of rows of the mask.     
 * @param n     The number of columns of the mask.
 *
 * @return      The vector of ring masks.
 */
const std::vector<cv::Mat> FourierSharpnessBase::assemble_ring_mask_vector(const int n) {
    std::vector<cv::Mat> ring_masks;
    std::vector<std::string> frequencies = {Constants::FREQ_BAND_MID,
                                            Constants::FREQ_BAND_HIGH,
                                            Constants::FREQ_BAND_HIGHEST};

    cv::Mat mask;
    for (auto const& frequency : frequencies) {
        mask = generate_ring_mask(n, frequency);
        ring_masks.push_back(mask);
        mask.release();
    }

    return ring_masks;
}

/**
 * Assembles a vector with one ring mask.
 *
 * @param m     The number of rows of the mask.     
 * @param n     The number of columns of the mask.
 *
 * @return      The vector of ring masks.
 */
const std::vector<cv::Mat> FourierSharpnessBase::assemble_single_ones_ring_mask(
                                                                    const int m,
                                                                    const int n) {
    cv::Mat ones = cv::Mat(m, n, CV_8UC1, cv::Scalar(255));
    std::vector<cv::Mat> ring_masks = {ones};
    return ring_masks;
}


/**
 * Pads the image with the specified value.
 *
 * @param img       The image to be transformed.
 * @param val       The padding value.
 *
 * @return          The padded image.
 */
cv::Mat FourierSharpnessBase::squarify(cv::Mat img, const double val) {
    const unsigned pad_size = cv::max(img.rows, img.cols);

    cv::Mat padded;
    cv::copyMakeBorder(img, padded, 0,
                       pad_size - img.rows, 0,
                       pad_size - img.cols, 
                       cv::BORDER_CONSTANT, 
                       cv::Scalar::all(0));
    return padded;
}        


/**
 * Pads the image with the specified value.
 *
 * @param m            The image to be transformed.
 * @param filename     The padding value.
 */
void FourierSharpnessBase::write_mat_to_file(cv::Mat& m, std::string const &filename) {
    std::ofstream fout(filename);

    if(fout) {

        for(unsigned i = 0; i < m.rows; i++) {
            for(unsigned j = 0; j < m.cols; j++) {
                fout << m.at<double>(i, j) << "\t";
            }
            fout << std::endl;
        }

        fout.close();
    } else {
        std::cout << "The file is closed." << std::endl;
    }
}


/**
 * Retrieves the sum of each element of eight radii 
 * of a spectrum in the form of vector<double>.
 *
 * @param spectrum      The FFT spectrum to be analysed.
 *
 * @return              A vector representing the coefficient
 *                      cumulative sum array.
 */
std::vector<double> FourierSharpnessBase::get_fft_coeff_vector(cv::Mat const &spectrum) {
    const unsigned n = spectrum.rows;

    const unsigned xc = n / 2;
    const unsigned yc = n / 2;

    std::vector<cv::Mat> masked_spectra;
    std::vector<std::vector<cv::Point>> indices;
    int lst = INT_MAX;

    draw_radial_vectors(masked_spectra,
                        indices,
                        spectrum,
                        lst,
                        xc,
                        yc,
                        n);  

    std::vector<double> sum = process_radial_vectors(masked_spectra, indices, lst);

    masked_spectra.clear();
    indices.clear();
    return sum;
}