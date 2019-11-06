#include "../lib/include/fft.hpp"
#include "../lib/include/fft_utils.hpp"
#include "../lib/include/helper.hpp"
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
    std::vector<cv::Mat> img_vec;
    std::string const path = argv[1];

    Helper *helper = new Helper();

    helper->open_dataset(img_vec, path);

    std::string save_path = "/home/victor/Documents/msc-image-database/callisia/gray/50/";

    cv::Mat gray;
    unsigned i = 1;
    for (auto &img : img_vec) {
        gray = helper->luminance(img);

        std::string name = to_string(i) + ".png";
        i++;

        cv::imwrite(save_path + name, gray);
        gray.release();
    }

    delete helper;
    img_vec.clear();

    return 0; 
}