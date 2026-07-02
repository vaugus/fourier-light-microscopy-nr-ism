/**
 * @file configuration.hpp
 * @brief Configuration class for runtime parameters
 */

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

class Configuration
{
public:
    static Configuration &instance()
    {
        static Configuration config;
        return config;
    }

    void load(const std::string &filename) {
        std::ifstream file(filename);

        if (!file)
        {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string line;

        while (std::getline(file, line))
        {
            auto equals_delimiter = line.find('=');

            std::string key = line.substr(0, equals_delimiter);
            std::string value = line.substr(equals_delimiter + 1);

            config[key] = std::stod(value);
        }


        std::cout << "Runtime configuration values:" << std::endl;
        show();
    }

    void load()
    {
        load("config.ini"); // Default configuration file
    }


    double getAngleStep() { return config["angle_step"]; }

    double getMaxAngle() { return config["max_angle"]; }

    double getResizeFactor() { return config["resize_factor"]; }

    double getClaheClipLimit() { return config["clahe_clip_limit"]; }

    double getClaheTilesGridSize() { return config["clahe_tiles_grid_size"]; }

    double getMinIqrValue() { return config["min_iqr_value"]; }

    double getNumRadialVectors() { return config["num_radial_vectors"]; }

    int getTotalAngles() {
        // Including from -2 * max_angle to +2 * max_angle gives 2*2 * max_angle/angle_step + 1
        
        // in this case, it means that the values were not initialized - return zero
        if (getMaxAngle() == 0.0 || getAngleStep() == 0.0) {
            return 0;
        }

        return static_cast<int>((2 * getMaxAngle()) / getAngleStep() + 1);
    }

    void setAngleStep(double step) { config["angle_step"] = step; }

    void setMaxAngle(double angle)
    {
        config["max_angle"] = angle;
        // Update num_radial_vectors accordingly
        config["num_radial_vectors"] = (config["nmax_angle"] - static_cast<int>(config["angle_step"])) / static_cast<int>(config["angle_step"]) + 1;
    }

    void setResizeFactor(double factor) { config["resize_factor"] = factor; }

    void setClaheClipLimit(double limit) { config["clahe_clip_limit"] = limit; }

    void setClaheTilesGridSize(double size) { config["clahe_tiles_grid_size"] = size; }

    void setMinIqrValue(double value) { config["min_iqr_value"] = value; }

    void setNumRadialVectors(double num) { config["num_radial_vectors"] = num; }

    /**
     * @brief angle_step: FFT radial angle step in degrees
     * Default: 5 degrees
     * 
     * @brief max_angle: Maximum radial angle in degrees
     * Default: 110 degrees
     * @brief resize_factor: Factor by which to resize images before FFT
     * Default: 0.5
     * @brief clahe_clip_limit: CLAE clip limit for contrast enhancement
     * Default: 2.0
     * @brief clahe_tiles_grid_size: CLAE tile grid size
     * Default: 8
     * @brief min_iqr_value: Minimum value for IQR calculation
     * Default: DBL_MIN equivalent (1e-16)
     * @brief num_radial_vectors: Number of radial vectors (computed from angle step and max angle)
     * Default: 22 (computed as (110 - 5) / 5 + 1 = 21, but typically 22 vectors)
     */
    void setDefaultValues()
    {
        config["angle_step"] = 5.0f;
        config["max_angle"] = 110;
        config["resize_factor"] = 0.5f;
        config["clahe_clip_limit"] = 2.0f;
        config["clahe_tiles_grid_size"] = 8;
        config["min_iqr_value"] = 1e-16;
        config["num_radial_vectors"] = 22;
    }

    void show()
    {
        std::cout << "angle_step = " << getAngleStep() << std::endl;
        std::cout << "max_angle = " << getMaxAngle() << std::endl;
        std::cout << "resize_factor = " << getResizeFactor() << std::endl;
        std::cout << "clahe_clip_limit = " << getClaheClipLimit() << std::endl;
        std::cout << "clahe_tiles_grid_size = " << getClaheTilesGridSize() << std::endl;
        std::cout << "min_iqr_value = " << getMinIqrValue() << std::endl;
        std::cout << "num_radial_vectors = " << getNumRadialVectors() << std::endl;
    }
private:
    Configuration() = default;
    ~Configuration() = default;

    // Prevent copying and moving
    Configuration(const Configuration &) = delete;
    Configuration &operator=(const Configuration &) = delete;
    Configuration(Configuration &&) = delete;
    Configuration &operator=(Configuration &&) = delete;

    std::unordered_map<std::string, double> config;
};