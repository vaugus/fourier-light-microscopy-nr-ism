/**
 * @file fourier_sharpness_base.hpp
 *
 * @brief Header file for class SampleFunctionBase.
 *
 * Header file with a class for operations concerning 
 * the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#ifndef FOURIER_SHARPNESS_BASE_HPP
#define FOURIER_SHARPNESS_BASE_HPP

#include <vector>

#include <opencv2/core.hpp>
#include "ftiqa/configuration.hpp"


/**
 * @class SampleFunctionBase
 *
 * @brief Organizes the Fourier IQA in methods
 *
 * This class contains operations concerning the Fourier Transform
 * based image quality assessment procedure. It is responsible for
 * organizing the descriptor generation in comprehensible methods.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class SampleFunctionBase {

    public:

        /** @brief Instantiates a new SampleFunctionBase object. */
        SampleFunctionBase();

        /** @brief Destroys a SampleFunctionBase instance. */
        ~SampleFunctionBase();

        /** @param smallest_vector_size     The smallest vector size to set.*/
        void setSmallestVectorSize(const int smallest_vector_size);
        
        /** @return current smallest vector size.*/
        int getSmallestVectorSize();
        
        /** 
         * @param xc    The x coordinate of the center point
         * @param yc    The y coordinate of the center point.
         */
        void setCenter(const unsigned xc, const unsigned yc);

        /** 
         * @return     A cv::Point object that represents the center of
         *             the current image. 
         */
        cv::Point getCenter();

        /** 
         * @param xc    The x coordinate of the center point
         * @param yc    The y coordinate of the center point.
         */
        void setRadialVectorMasks(std::vector<cv::Mat> const& radial_vector_masks);
        
        /** 
         * @return      A std::vector of cv::Mat objects with the radial vector masks
         *              of the current dimensions.
         */
        std::vector<cv::Mat> getRadialVectorMasks();

        /** 
         * @param indices   Two-dimensional std::vector of cv::Point with the indices
         *                  of white points in a radial vector mask.  
         */
        void setIndices(std::vector<std::vector<cv::Point>> &indices);

        /** 
         * @return      A two-dimensional std::vector of cv::Point with the current 
         *              indices of white points. 
         */
        std::vector<std::vector<cv::Point>> getIndices();

        /** 
         * @brief Performs the Discrete Fourier Transform on the given image.
         *
         * @param image             The RGB image to be transformed.
         * @param gray_spectrum     A reference to a cv::Mat object which will
         *                          receive the Fourier spectrum of the image.
         */
        virtual void fft(cv::Mat const &image, cv::Mat &gray_spectrum);

        /** 
         * @brief Creates the class' radial vector masks with the given n.
         *
         * @param n     The dimension of the square image.
         */
        void generateRadialVectors(const int n);

        /** 
         * @brief Computes the mean of each element from the descriptor.
         *
         * @param masked_spectra    Each spectrum multiplied by each mask.
         *
         * @return      The vector which represents the descriptor.
         */
        std::vector<double> processRadialVectors(std::vector<cv::Mat> &masked_spectra);


        /** 
         * @brief Parametrizes the descriptor computing procedure.
         *
         * @param spectrum      The spectrum from where the descriptor will be extracted.
         *
         * @return      The vector which represents the descriptor.
         */
        std::vector<double> computeSampleFunction(cv::Mat const &spectrum);

    private:

        /** Cosines of each angle to compute the point locations. */
        inline static std::vector<double> cosines;
        
        /** Sines of each angle to compute the point locations. */
        inline static std::vector<double> sines;
        
        /** Set of radial vector masks to be applied in spectra. */
        inline static std::vector<cv::Mat> radial_vector_masks;
        
        /** Set of the (x,y) positions of white points in each 
         *  radial vector mask. */
        inline static std::vector<std::vector<cv::Point>> indices;
        
        /** The smallest vector size among all radial vectors. */
        inline static int smallest_vector_size;
        
        /** The center of the spectrum. */
        inline static cv::Point center;

        /** 
         * @brief Initializes the constant values and arrays.
         *
         * @param step      The increment angles to compute sin and cos.
         * @param limit     The maximum angle.
         */
        void initializeConstants(int const step, int const limit);

        /** 
         * @brief Multiplies element-wise each radial vector mask by 
         *        the given spectrum.
         *
         * @param spectrum    The spectrum to be masked.
         *
         * @return      The vector with the spectrum multiplied by each of the masks.
         */
        std::vector<cv::Mat> applyRadialVectorMasks(cv::Mat const &spectrum);

        /** 
         * @brief Reduces the size of all indices to the smallest vector size.
         */
        void cropIndices();
};

#endif
