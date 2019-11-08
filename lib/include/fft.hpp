#ifndef FFT_HPP
#define FFT_HPP

#include "fourier_sharpness_base.hpp"

/**
 * Header file with a class for operations concerning 
 * the blur analysis in the Fourier domain.
 *
 * @author Victor Augusto
 * @version 1.0
 */
class FFT : public FourierSharpnessBase {

    public:

        void run_batch_fft(std::vector<cv::Mat> &img_vec);
        void sort_energy_distances(std::vector<std::pair<int, double>> &arr);

    private:

        unsigned get_final_spectrum_size(cv::Mat const &img);
};

#endif