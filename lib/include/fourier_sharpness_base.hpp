/**
 * \file fourier_sharpness_base.hpp
 *
 * \brief Header file for class FourierSharpnessBase.
 *
 * Header file with a class for operations concerning 
 * the blur analysis in the Fourier domain.
 *
 * \author Victor Augusto 
 * \version 1.0
 * \date 2019-11-21
 */

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
 * \class FourierSharpnessBase
 *
 * \brief Organizes the Fourier IQA in methods
 *
 * This class contains operations concerning the Fourier Transform
 * based image quality assessment procedure. It is responsible for
 * organizing the descriptor generation in comprehensible methods.
 *
 * \author Victor Augusto
 * \version 1.0
 */
class FourierSharpnessBase {

    public:

        /** \brief Instantiates a new FourierSharpnessBase object. */
        FourierSharpnessBase();

        /** \brief Destroys a FourierSharpnessBase instance. */
        ~FourierSharpnessBase();

        /** \param smallest_vector_size     The smallest vector size to set.*/
        void set_smallest_vector_size(const int smallest_vector_size);
        
        /** \return current smallest vector size.*/
        int get_smallest_vector_size();
        
        /** 
         * \param xc    The x coordinate of the center point
         * \param yc    The y coordinate of the center point.
         */
        void set_center(const unsigned xc, const unsigned yc);

        /** 
         * \return     A cv::Point object that represents the center of
         *             the current image. 
         */
        cv::Point get_center();

        /** 
         * \param xc    The x coordinate of the center point
         * \param yc    The y coordinate of the center point.
         */
        void set_radial_vector_masks(std::vector<cv::Mat> const& radial_vector_masks);
        
        /** 
         * \return      A std::vector of cv::Mat objects with the radial vector masks
         *              of the current dimensions.
         */
        std::vector<cv::Mat> get_radial_vector_masks();

        /** 
         * \param indices   Two-dimensional std::vector of cv::Point with the indices
         *                  of white points in a radial vector mask.  
         */
        void set_indices(std::vector<std::vector<cv::Point>> &indices);

        /** 
         * \return      A two-dimensional std::vector of cv::Point with the current 
         *              indices of white points. 
         */
        std::vector<std::vector<cv::Point>> get_indices();

        /** 
         * \brief Performs the Discrete Fourier Transform on the given image.
         *
         * \param image             The RGB image to be transformed.
         * \param gray_spectrum     A reference to a cv::Mat object which will
         *                          receive the Fourier spectrum of the image.
         */
        virtual void fft(cv::Mat const &image, cv::Mat &gray_spectrum);

        void generate_radial_vectors(const int n);

        void draw_radial_vectors(std::vector<cv::Mat> &masked_spectra,
                                 std::vector<std::vector<cv::Point>> &indices,
                                 cv::Mat const &spectrum,
                                 int &lst,
                                 const int xc,
                                 const int yc,
                                 const int n);

        std::vector<double> process_radial_vectors(std::vector<cv::Mat> &masked_spectra,
                                                   std::vector<std::vector<cv::Point>> &indices,
                                                   unsigned lst);

        std::vector<double> process_radial_vectors(std::vector<cv::Mat> &masked_spectra);
        
        std::vector<double> compute_descriptor(cv::Mat const &spectrum);

    private:

        inline static std::vector<double> cosines;
        inline static std::vector<double> sines;
        inline static std::vector<cv::Mat> radial_vector_masks;
        inline static std::vector<std::vector<cv::Point>> indices;
        inline static int smallest_vector_size;
        inline static cv::Point center;

        void initialize_constants(int const step, int const limit);

        std::vector<cv::Mat> apply_radial_vector_masks(cv::Mat const &spectrum);
        void crop_indices();
};

#endif