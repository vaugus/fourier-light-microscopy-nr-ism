/**
 * @file helper.cpp
 *
 * @brief Source file for the implementation of the
 * Helper class.
 *
 * Implementation of the Helper class, which covers
 * methods of general use in any other class.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#include "ftiqa/helper.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

/** 
 * Opens each image of the dataset, stores it in a vector, retrieves
 * the name of each file and stores it in a vector.
 */
void Helper::open_dataset(std::vector<cv::Mat> &img_vec, std::string const& path) {
    std::string line;
    std::ifstream file (path);

    std::getline(file, line);
    const unsigned qtd = std::stoi(line);
    cv::Mat tmp;

    for (unsigned i = 0; i < qtd; i++) {
        // read the line
        std::getline(file, line);

        // read the image file
        tmp = cv::imread(line);

        // insert the image in the vector
        img_vec.push_back(tmp);    
        
        tmp.release();
    }

    file.close();
}

/** 
 * Takes the image of the [0, 255] range, normalizes it 
 * to the [0,1] interval and finally returns a float
 * Mat object with the image data.
 */
cv::Mat Helper::as_array(cv::Mat &img) {
    cv::Mat arr;

    // scale values to 32-bit floats
    img = img / 255.0;
    
    // convert to 3-channel 32-bit float
    img.convertTo(arr, CV_32FC3);

    return arr;
}

/**
 * Takes the array of floating point numbers (either float or
 * double) performs a min-max normalization if the normalize parameter
 * is true and then to the [0, 255] range. Finally, returns a Mat object
 * with the array data.
 */
cv::Mat Helper::as_image(cv::Mat const array, bool normalize) {
    cv::Mat tmp;
    cv::Mat img;

    if (normalize) {
        cv::normalize(array, tmp, 0, 1, cv::NORM_MINMAX, CV_32F);
    }

    // scale values to 8-bit unsigned integers
    tmp = tmp * 255;

    // convert to 3-channel 8-bit unsigned integers (RGB image)
    tmp.convertTo(img, CV_8UC3);

    tmp.release();
    return img;
}

/**
 * Converts a RGBA digital image to a grayscale image,
 * using the luminance method.
 *
 * L = 0.299R + 0.587G + 0.114B
 */
cv::Mat Helper::luminance(cv::Mat const& img) {
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    return gray;
}
