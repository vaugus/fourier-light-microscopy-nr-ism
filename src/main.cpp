#include "../lib/include/sample_function.hpp"
#include "../lib/include/fft_utils.hpp"
#include "../lib/include/helper.hpp"
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
    std::vector<cv::Mat> img_vec;
    std::string const path = argv[1];

    SampleFunction *sample_function = new SampleFunction();
    Helper *helper = new Helper();

    helper->open_dataset(img_vec, path);

    sample_function->run_batch_fft(img_vec);

    delete sample_function;
    delete helper;
    img_vec.clear();

    return 0; 
}
