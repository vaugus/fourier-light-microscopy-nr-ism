#include "../lib/include/sample_function.hpp"
#include "../lib/include/fft_utils.hpp"
#include "../lib/include/helper.hpp"
#include <memory>
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image_path> <output_path>" << std::endl;
        return 1;
    }
    
    try {
        std::vector<cv::Mat> img_vec;
        std::string const path = argv[1];

        std::unique_ptr<SampleFunction> sample_processor = std::make_unique<SampleFunction>();
        std::unique_ptr<Helper> helper_processor = std::make_unique<Helper>();

        helper_processor->open_dataset(img_vec, path);
        sample_processor->run_batch_fft(img_vec);
        img_vec.clear();
    } catch (const std::exception& e) {
        std::cerr << "An exception occurred during execution: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}
