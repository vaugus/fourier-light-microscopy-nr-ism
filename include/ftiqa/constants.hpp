#pragma once

#include <string>

namespace Constants {

    // methods
    inline const std::string METHOD_FFT = "fft";
    inline const std::string METHOD_STFT = "stft";

    // window functions
    inline const std::string WINDOW_HANN = "hann";
    inline const std::string WINDOW_GAUSSIAN = "gaussian";
    inline const std::string WINDOW_FLATTOP = "flattop";
    inline const std::string WINDOW_BLACKMANHARRIS = "blackmanharris";

    // colour spaces
    inline const std::string COLOURSPACE_GRAYSCALE = "grayscale";
    inline const std::string COLOURSPACE_HSV = "hsv";

    // frequency bands
    inline const std::string FREQ_BAND_MID = "mid";
    inline const std::string FREQ_BAND_HIGH = "high";
    inline const std::string FREQ_BAND_HIGHEST = "highest";

    // error messages
    inline const std::string ERROR_INVALID_DIMENSIONS_WINDOW = "Invalid dimensions";

    // possible parameter elements of FFT classification 
    inline const std::string ELEMENT_ENERGY_MID = "mid";
    inline const std::string ELEMENT_ENERGY_HIGH = "high";
    inline const std::string ELEMENT_ENERGY_HIGHEST = "highest";
};
