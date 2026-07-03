#ifndef IMAGE_UTIL_HPP
#define IMAGE_UTIL_HPP

#include <filesystem>
#include <vector>

#include <opencv2/core.hpp>

namespace imageutil
{
    std::vector<cv::Mat> loadDataset(const std::filesystem::path &datasetFile);
    cv::Mat luminance(const cv::Mat &image);
}

#endif
