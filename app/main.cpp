#include <iostream>
#include <memory>

#include "ftiqa/image_util.hpp"
#include "ftiqa/sample_function.hpp"
#include "ftiqa/configuration.hpp"

int main(int argc, char **argv) {
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image_path> <output_path>" << std::endl;
        return 1;
    }
    
    try {
        std::string const path = argv[1];

        Configuration::instance().load();

        std::unique_ptr<SampleFunction> sample_processor = std::make_unique<SampleFunction>();
        
        std::vector<cv::Mat> img_vec = imageutil::loadDataset(path);

        sample_processor->runBatchFFT(img_vec);
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
