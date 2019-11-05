#pragma once

namespace Constants {

    // methods
    const std::string METHOD_FFT = "fft";
    const std::string METHOD_STFT = "stft";

    // window functions
    const std::string WINDOW_HANN = "hann";
    const std::string WINDOW_GAUSSIAN = "gaussian";
    const std::string WINDOW_FLATTOP = "flattop";
    const std::string WINDOW_BLACKMANHARRIS = "blackmanharris";

    // colour spaces
    const std::string COLOURSPACE_GRAYSCALE = "grayscale";
    const std::string COLOURSPACE_HSV = "hsv";

    // frequency bands
    const std::string FREQ_BAND_MID = "mid";
    const std::string FREQ_BAND_HIGH = "high";
    const std::string FREQ_BAND_HIGHEST = "highest";

    // error messages
    const std::string ERROR_INVALID_DIMENSIONS_WINDOW = "Invalid dimensions";

    // possible parameter elements of FFT classification 
    const std::string ELEMENT_ENERGY_MID = "mid";
    const std::string ELEMENT_ENERGY_HIGH = "high";
    const std::string ELEMENT_ENERGY_HIGHEST = "highest";
};