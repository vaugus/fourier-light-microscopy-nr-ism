#ifndef IMAGE_UTIL_HPP
#define IMAGE_UTIL_HPP

#include <fstream>
#include <filesystem>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

namespace imageutil
{
    std::vector<cv::Mat> load_dataset(const std::filesystem::path &dataset_file);
    cv::Mat luminance(const cv::Mat &image);
}

#endif
