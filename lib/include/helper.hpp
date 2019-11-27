/**
 * @file helper.hpp
 *
 * @brief Header file for class Helper.
 *
 * Header file with a class that provides functionalities
 * of common use among other classes.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#ifndef HELPER_HPP
#define HELPER_HPP

#include <fstream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

/**
 * @class Helper
 *
 * @brief Provides broadly used functionalities. 
 *
 * Contains either methods of general use or methods
 * that had no other context to be inserted into.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class Helper {

    public:

        /**
         * @brief Opens all the files from the input file, read by the given path.
         * 
         * @param img_vec   Vector where the images will be stored.
         * @param path      String with the path to the input file.
         */
        void open_dataset(std::vector<cv::Mat> &img_vec, std::string const& path);
        
        /**
         * @brief Converts the digital image to an array of floats.
         * 
         * @param img   The image to be converted.
         *
         * @return      The converted image as a Mat object.
         */
        cv::Mat as_array(cv::Mat &img);

        /**
         * @brief Converts an array of floats to a digital image.
         * 
         * @param array         The array to be converted.
         * @param normalize     Boolean to determine if the conversion should
         *                      be normalized or not.
         *
         * @return              The converted array as a Mat object.
         */
        cv::Mat as_image(cv::Mat const array, bool normalize);

        /**
         * @brief Converts an image to the grayscale colour space.
         * 
         * @param img   Reference to the image to be converted.
         *   
         * @return      The grayscale image.
         */
        cv::Mat luminance(cv::Mat const& img);
};

#endif