#include "ftiqa/image_util.hpp"

#include <fstream>
#include <stdexcept>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace imageutil
{
    std::vector<cv::Mat> load_dataset(const std::filesystem::path &dataset_file)
    {
        std::ifstream file(dataset_file);

        if (!file)
        {
            throw std::runtime_error("Cannot open dataset file: " + dataset_file.string());
        }

        std::vector<cv::Mat> images;
        std::string line;

        while (std::getline(file, line))
        {
            auto image = cv::imread(line);

            if (image.empty())
            {
                throw std::runtime_error("Cannot load image: " + line);
            }

            images.push_back(std::move(image));
        }

        return images;
    }

    cv::Mat luminance(const cv::Mat &image)
    {
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        return gray;
    }
}
