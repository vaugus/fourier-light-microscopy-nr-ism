#include "../include/helper.hpp"

/**
 * Implementation of the Helper class.
 *
 * @author Victor Augusto
 * @version 1.0
 */


/** 
    * Opens each image of the dataset, stores it in a vector, retrieves
    * the name of each file and stores it in a vector.
    *
    * @param img_vec   reference to the std::vector that will store
    *                  all the input images
    * @param path      the name of the input text file which contains
    *                  strings for all the input images
    * @since           1.0
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


cv::Mat Helper::as_array(cv::Mat &img) {
    cv::Mat arr;

    // scale values to 32-bit floats
    img = img / 255.0;
    
    // convert to 3-channel 32-bit float
    img.convertTo(arr, CV_32FC3);

    return arr;
}


cv::Mat Helper::as_image(cv::Mat const arr, bool normalize) {
    cv::Mat tmp;
    cv::Mat img;


    if (normalize) {
        cv::normalize(arr, tmp, 0, 1, cv::NORM_MINMAX, CV_32F);
    }

    // scale values to 8-bit unsigned integers
    tmp = tmp * 255;

    // convert to 3-channel 8-bit unsigned integers (RGB image)
    tmp.convertTo(img, CV_8UC3);

    tmp.release();
    return img;
}


/**
 * Converts a RGBA image to the grayscale colorspace.
 *
 * @param rgba          The input image to be converted.
 * 
 * @return              The converted image.
*/
cv::Mat Helper::luminance(cv::Mat const& img) {
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    return gray;
}


/**
 * Performs a resizing procedure on a grayscale image,
 * considering the provided scale factor.
 *
 * @param img          The input image to be converted.
 * @param factor       The colour space of the output.
 * 
 * @return             The resized image.
*/
void Helper::resize_to_factor(cv::Mat const& gray, cv::Mat &output, const unsigned factor) {
    cv::Size size(gray.cols / factor,  gray.rows / factor);
    cv::resize(gray, output, size);
}


/**
 * Performs a Contrast Limited Adaptive Histogram Equalization
 * (aka CLAHE) procedure on an image.
 *
 * @param img           The input image to be equalised.
 * @param output        The empty output image.
 * 
 * @return              The equalised image.
*/
void Helper::perform_clahe(cv::Mat const& img, cv::Mat &output) {
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->apply(img, output);
}