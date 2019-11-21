#ifndef HELPER_HPP
#define HELPER_HPP

#include <fstream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

/**
 * 
 * @author      Victor Augusto Alves Catanante
 * @version     1.0
 * @since       1.0
*/
class Helper {

    public:

        void open_dataset(std::vector<cv::Mat> &img_vec, std::string const& path);
        
        cv::Mat as_array(cv::Mat &img);

        cv::Mat as_image(cv::Mat const arr, bool normalize);

        cv::Mat luminance(cv::Mat const& img);
};

#endif