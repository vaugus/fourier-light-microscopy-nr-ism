#ifndef FOURIER_SHARPNESS_BASE_HPP
#define FOURIER_SHARPNESS_BASE_HPP

#include <map>
#include <algorithm>
#include <numeric>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/photo.hpp>
#include "../include/helper.hpp"


/**
 * Header file with a class for operations concerning 
 * the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class FourierSharpnessBase {

    public:

        FourierSharpnessBase();
        ~FourierSharpnessBase();

        void set_smallest_vector_size(const int smallest_vector_size);
        int get_smallest_vector_size();
        
        void set_center(const unsigned xc, const unsigned yc);
        cv::Point get_center();

        void set_radial_vector_masks(std::vector<cv::Mat> const& radial_vector_masks);
        std::vector<cv::Mat> get_radial_vector_masks();

        void set_indices(std::vector<std::vector<cv::Point>> const& indices);
        std::vector<std::vector<cv::Point>> get_indices();

        virtual void fft(cv::Mat const &rgba, cv::Mat &gray_spectrum);

        std::vector<double> band_energy(cv::Mat const& spectrum, 
                                        std::vector<cv::Mat> const& ring_masks);

        const cv::Mat generate_ring_mask(const int n, std::string const& frequency_band);

        void generate_radial_vectors(const int n);

        void draw_radial_vectors(std::vector<cv::Mat> &masked_spectra,
                                 std::vector<std::vector<cv::Point>> &indices,
                                 cv::Mat const &spectrum,
                                 int &lst,
                                 const int xc,
                                 const int yc,
                                 const int n);

        std::vector<double> process_radial_vectors(std::vector<cv::Mat> const &masked_spectra,
                                                   std::vector<std::vector<cv::Point>> &indices,
                                                   unsigned lst);
        
        const std::vector<cv::Mat> assemble_ring_mask_vector(const int n);

        const std::vector<cv::Mat> assemble_single_ones_ring_mask(const int m, const int n);
        
        cv::Mat squarify(cv::Mat img, const double val);

        void write_mat_to_file(cv::Mat& m, std::string const &filename);

        std::vector<double> get_fft_coeff_vector(cv::Mat const &spectrum);

    private:

        inline static std::vector<double> cosines;
        inline static std::vector<double> sines;
        inline static std::vector<cv::Mat> radial_vector_masks;
        inline static std::vector<std::vector<cv::Point>> indices;
        inline static int smallest_vector_size;
        inline static cv::Point center;

        void initialize_constants(int const step, int const limit);
};

#endif