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
            auto equalsDelimiter = line.find('=');

            std::string key = line.substr(0, equalsDelimiter);
            std::string value = line.substr(equalsDelimiter + 1);

            config[key] = std::stod(value);
        }


        std::cout << "Runtime configuration values:" << std::endl;
        show();
    }

    void load()
    {
        load("config.ini"); // Default configuration file
    }


    double getAngleStep() { return config["angleStep"]; }

    double getMaxAngle() { return config["maxAngle"]; }

    double getResizeFactor() { return config["resizeFactor"]; }

    double getClaheClipLimit() { return config["claheClipLimit"]; }

    double getClaheTilesGridSize() { return config["claheTilesGridSize"]; }

    double getMinIqrValue() { return config["minIqrValue"]; }

    double getNumRadialVectors() { return config["numRadialVectors"]; }

    int getTotalAngles() {
        // Including from -2 * maxAngle to +2 * maxAngle gives 2*2 * maxAngle/angleStep + 1
        
        // in this case, it means that the values were not initialized - return zero
        if (getMaxAngle() == 0.0 || getAngleStep() == 0.0) {
            return 0;
        }

        return static_cast<int>((2 * getMaxAngle()) / getAngleStep() + 1);
    }

    void setAngleStep(double step) { config["angleStep"] = step; }

    void setMaxAngle(double angle)
    {
        config["maxAngle"] = angle;
        // Update numRadialVectors accordingly
        config["numRadialVectors"] = (config["maxAngle"] - static_cast<int>(config["angleStep"])) / static_cast<int>(config["angleStep"]) + 1;
    }

    void setResizeFactor(double factor) { config["resizeFactor"] = factor; }

    void setClaheClipLimit(double limit) { config["claheClipLimit"] = limit; }

    void setClaheTilesGridSize(double size) { config["claheTilesGridSize"] = size; }

    void setMinIqrValue(double value) { config["minIqrValue"] = value; }

    void setNumRadialVectors(double num) { config["numRadialVectors"] = num; }

    /**
     * @brief angleStep: FFT radial angle step in degrees
     * Default: 5 degrees
     * @brief maxAngle: Maximum radial angle in degrees
     * Default: 110 degrees
     * @brief resizeFactor: Factor by which to resize images before FFT
     * Default: 0.5
     * @brief claheClipLimit: CLAE clip limit for contrast enhancement
     * Default: 2.0
     * @brief claheTilesGridSize: CLAE tile grid size
     * Default: 8
     * @brief minIqrValue: Minimum value for IQR calculation
     * Default: DBL_MIN equivalent (1e-16)
     * @brief numRadialVectors: Number of radial vectors (computed from angle step and max angle)
     * Default: 22 (computed as (110 - 5) / 5 + 1 = 21, but typically 22 vectors)
     */
    void setDefaultValues()
    {
        config["angleStep"] = 5.0f;
        config["maxAngle"] = 110;
        config["resizeFactor"] = 0.5f;
        config["claheClipLimit"] = 2.0f;
        config["claheTilesGridSize"] = 8;
        config["minIqrValue"] = 1e-16;
        config["numRadialVectors"] = 22;
    }

    void show()
    {
        std::cout << "angleStep = " << getAngleStep() << std::endl;
        std::cout << "maxAngle = " << getMaxAngle() << std::endl;
        std::cout << "resizeFactor = " << getResizeFactor() << std::endl;
        std::cout << "claheClipLimit = " << getClaheClipLimit() << std::endl;
        std::cout << "claheTilesGridSize = " << getClaheTilesGridSize() << std::endl;
        std::cout << "minIqrValue = " << getMinIqrValue() << std::endl;
        std::cout << "numRadialVectors = " << getNumRadialVectors() << std::endl;
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