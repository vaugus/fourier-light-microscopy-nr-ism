/**
 * @file configuration.hpp
 * @brief Configuration class for runtime parameters
 */

#pragma once

#include <string>

class Configuration
{
public:
    /**
     * @brief FFT radial angle step in degrees
     * Default: 5 degrees
     */
    float angle_step = 5.0f;

    /**
     * @brief Maximum radial angle in degrees
     * Default: 110 degrees
     */
    float max_angle = 110.0f;

    /**
     * @brief Factor by which to resize images before FFT
     * Default: 0.5
     */
    float resize_factor = 0.5f;

    /**
     * @brief CLAE clip limit for contrast enhancement
     * Default: 2.0
     */
    float clahe_clip_limit = 2.0f;

    /**
     * @brief CLAE tile grid size
     * Default: 8
     */
    int clahe_tiles_grid_size = 8;

    /**
     * @brief Minimum value for IQR calculation
     * Default: DBL_MIN equivalent (1e-16)
     */
    double min_iqr_value = 1e-16;

    /**
     * @brief Number of radial vectors (computed from angle step and max angle)
     * Default: 22 (computed as (110 - 5) / 5 + 1 = 21, but typically 22 vectors)
     */
    int num_radial_vectors = 22;

    float getAngleStep() const { return angle_step; }

    int getMaxAngle() const { return max_angle; }

    float getResizeFactor() const { return resize_factor; }

    float getClaheClipLimit() const { return clahe_clip_limit; }

    int getClaheTilesGridSize() const { return clahe_tiles_grid_size; }

    double getMinIqrValue() const { return min_iqr_value; }

    int getNumRadialVectors() const { return num_radial_vectors; }

    int getTotalAngles() const
    {
        // Including from -2 * max_angle to +2 * max_angle gives 2*2 * max_angle/angle_step + 1
        return (2 * max_angle) / angle_step + 1;
    }

    void setAngleStep(float step) { angle_step = step; }

    void setMaxAngle(int angle)
    {
        max_angle = angle;
        // Update num_radial_vectors accordingly
        num_radial_vectors = (max_angle - static_cast<int>(angle_step)) / static_cast<int>(angle_step) + 1;
    }

    void setResizeFactor(float factor) { resize_factor = factor; }

    void setClaheClipLimit(float limit) { clahe_clip_limit = limit; }

    void setClaheTilesGridSize(int size) { clahe_tiles_grid_size = size; }

    void setMinIqrValue(double value) { min_iqr_value = value; }

    void setNumRadialVectors(int num) { num_radial_vectors = num; }

    /// TODO
    bool loadFromFile(const std::string &filepath)
    {
        return true;
    }

    /// TODO
    bool saveToFile(const std::string &filepath) const
    {
        return true;
    }

    /**
     * @brief Clear all configuration values to defaults
     */
    void clear()
    {
        angle_step = 5.0f;
        max_angle = 110;
        resize_factor = 0.5f;
        clahe_clip_limit = 2.0f;
        clahe_tiles_grid_size = 8;
        min_iqr_value = 1e-16;
        num_radial_vectors = 22;
    }
};