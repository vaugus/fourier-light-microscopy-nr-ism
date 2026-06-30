#include <gtest/gtest.h>
#include "ftiqa/image_util.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace imageutil;
using namespace cv::utils::logging;

// Test fixture for image utility tests
class ImageUtilTest : public ::testing::Test
{
protected:
    std::string test_images_dir;
    std::string test_dataset_file;

    void SetUp() override
    {
        test_images_dir = "/tmp/ftiqa_test_images";
        test_dataset_file = "/tmp/ftiqa_test_images.txt";

        // Clean up any existing directories
        if (std::filesystem::exists(test_images_dir))
        {
            std::filesystem::remove_all(test_images_dir);
        }
    }

    void TearDown() override
    {
        // Clean up
        if (std::filesystem::exists(test_images_dir))
        {
            std::filesystem::remove_all(test_images_dir);
        }
        if (std::filesystem::exists(test_dataset_file))
        {
            std::filesystem::remove(test_dataset_file);
        }
    }
};

static std::filesystem::path executable_path()
{
    char buffer[4096];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

    if (len == -1)
        throw std::runtime_error("Failed to determine executable path");

    buffer[len] = '\0';
    return std::filesystem::path(buffer);
}

static void createTestImageDirectory(const std::string &test_image_directory) 
{
    if (!std::filesystem::exists(test_image_directory))
    {
        std::filesystem::create_directory(test_image_directory);
    }
}

// Helper function to create simple test images
static void createTestImage(const std::string &filename, cv::Scalar color)
{
    cv::Mat image(100, 100, CV_8UC3, color);
    cv::imwrite(filename, image);
}

// Helper function to create test dataset file
static void createTestDataset(const std::string &dataset_file, const std::vector<std::string> &image_paths)
{
    std::ofstream file(dataset_file);
    for (const auto &path : image_paths)
    {
        file << path << "\n";
    }
}

TEST_F(ImageUtilTest, LuminanceConversionBlueImage)
{
    // Create a test BGR image - pure blue
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(0, 0, 255));

    // Convert to grayscale using our function
    cv::Mat gray = luminance(image);

    // Verify dimensions
    EXPECT_EQ(gray.cols, 100);
    EXPECT_EQ(gray.rows, 100);
    EXPECT_EQ(gray.channels(), 1);
    EXPECT_EQ(gray.type(), CV_8UC1);

    // Verify luminance calculation for blue image
    double pixel_value = gray.at<uchar>(50, 50);
    // Should be in valid range
    EXPECT_LE(pixel_value, 255);
    EXPECT_GE(pixel_value, 0);
}

TEST_F(ImageUtilTest, LuminanceConversionWhiteImage)
{
    // Create a test BGR image - pure white
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(255, 255, 255));

    // Convert to grayscale
    cv::Mat gray = luminance(image);

    // White should have maximum luminance
    double pixel_value = gray.at<uchar>(0, 0);
    EXPECT_EQ(pixel_value, 255);
}

TEST_F(ImageUtilTest, LuminanceConversionBlackImage)
{
    // Create a test BGR image - pure black
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(0, 0, 0));

    // Convert to grayscale
    cv::Mat gray = luminance(image);

    // Black should have zero luminance
    double pixel_value = gray.at<uchar>(0, 0);
    EXPECT_EQ(pixel_value, 0);
}

TEST_F(ImageUtilTest, LuminanceConversionRedImage)
{
    // Create a test BGR image - pure red
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(255, 0, 0));

    // Convert to grayscale
    cv::Mat gray = luminance(image);

    // Verify output dimensions
    EXPECT_EQ(gray.size(), cv::Size(100, 100));
    EXPECT_EQ(gray.channels(), 1);
}

TEST_F(ImageUtilTest, LuminanceConversionGreenImage)
{
    // Create a test BGR image - pure green
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(0, 255, 0));

    // Convert to grayscale
    cv::Mat gray = luminance(image);

    // Verify output dimensions
    EXPECT_EQ(gray.size(), cv::Size(100, 100));
    EXPECT_EQ(gray.channels(), 1);
}

TEST_F(ImageUtilTest, LuminanceConversionGrayImage)
{
    // Create a test BGR image - pure gray
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(128, 128, 128));

    // Convert to grayscale
    cv::Mat gray = luminance(image);

    // Verify output dimensions
    EXPECT_EQ(gray.size(), cv::Size(100, 100));
    EXPECT_EQ(gray.channels(), 1);
}

TEST_F(ImageUtilTest, LoadDatasetMultipleImages) {
    // Create test directory and images
    test_images_dir = "/tmp/ftiqa_test_images2";
    test_dataset_file = "/tmp/ftiqa_test_images2.txt";

    // Create test images
    std::string image1 = test_images_dir + "/image1.png";
    std::string image2 = test_images_dir + "/image2.png";
    std::string image3 = test_images_dir + "/image3.png";

    createTestImageDirectory(test_images_dir);

    createTestImage(image1, cv::Scalar(255, 0, 0));
    createTestImage(image2, cv::Scalar(0, 255, 0));
    createTestImage(image3, cv::Scalar(0, 0, 255));

    // Create dataset file
    createTestDataset(test_dataset_file, {image1, image2, image3});

    // Load dataset
    std::vector<cv::Mat> images = loadDataset(test_dataset_file);

    // Verify we loaded all images
    EXPECT_EQ(images.size(), 3u);

    // Verify image dimensions
    EXPECT_EQ(images[0].size(), cv::Size(100, 100));
    EXPECT_EQ(images[1].size(), cv::Size(100, 100));
    EXPECT_EQ(images[2].size(), cv::Size(100, 100));
}

TEST_F(ImageUtilTest, LoadDatasetWithSingleImage) {
    // Create single image test
    test_images_dir = "/tmp/ftiqa_test_images3";
    test_dataset_file = "/tmp/ftiqa_test_images3.txt";

    createTestImageDirectory(test_images_dir);

    std::string single_image = test_images_dir + "/single.png";
    createTestImage(single_image, cv::Scalar(128, 128, 128));

    createTestDataset(test_dataset_file, {single_image});

    std::vector<cv::Mat> images = loadDataset(test_dataset_file);

    EXPECT_EQ(images.size(), 1u);
    EXPECT_EQ(images[0].size(), cv::Size(100, 100));
}

TEST_F(ImageUtilTest, LoadDatasetWithEmptyFile) {
    // Test with empty dataset file
    test_dataset_file = "/tmp/ftiqa_test_images_empty.txt";
    std::ofstream file(test_dataset_file);
    // Don't write anything, leave empty

    std::vector<cv::Mat> images = loadDataset(test_dataset_file);

    // Empty file should load empty vector
    EXPECT_EQ(images.size(), 0u);
}

TEST_F(ImageUtilTest, LoadDatasetWithNonExistentFile) {
    // Test with non-existent file
    test_dataset_file = "/nonexistent/path/to/images.txt";

    EXPECT_THROW(loadDataset(test_dataset_file), std::runtime_error);
}

TEST_F(ImageUtilTest, LoadDatasetWithInvalidImagePath) {
    // Create test file with one invalid image path
    test_images_dir = "/tmp/ftiqa_test_images4";
    test_dataset_file = "/tmp/ftiqa_test_images4.txt";

    createTestImageDirectory(test_images_dir);

    // Create only one image
    std::string valid_image = test_images_dir + "/valid.png";
    createTestImage(valid_image, cv::Scalar(128, 128, 128));

    // Add invalid path
    createTestDataset(test_dataset_file, {valid_image, "/nonexistent/image.png"});

    auto oldLevel = getLogLevel();
    setLogLevel(LOG_LEVEL_SILENT);

    // Should throw exception
    EXPECT_THROW(loadDataset(test_dataset_file), std::runtime_error);

    setLogLevel(oldLevel);
}

TEST_F(ImageUtilTest, LoadDatasetWithEmptyLines) {
    // Test dataset file with empty lines at end
    test_images_dir = "/tmp/ftiqa_test_images5";
    test_dataset_file = "/tmp/ftiqa_test_images5.txt";

    std::string image1 = test_images_dir + "/image1.png";
    createTestImageDirectory(test_images_dir);
    createTestImage(image1, cv::Scalar(128, 128, 128));

    std::ofstream file(test_dataset_file);
    file << image1 << "\n";
    file << "\n";  // Empty line
    file << "\n";  // Another empty line
    file.close();

    auto oldLevel = getLogLevel();
    setLogLevel(LOG_LEVEL_SILENT);
    
    // Should throw exception
    EXPECT_THROW(loadDataset(test_dataset_file), std::runtime_error);

    setLogLevel(oldLevel);
}

TEST_F(ImageUtilTest, LoadDatasetLargeDataset) {
    // Create a larger dataset to test performance and handling
    test_images_dir = "/tmp/ftiqa_test_images6";
    test_dataset_file = "/tmp/ftiqa_test_images6.txt";

    const size_t number_of_images = 5000;
    std::vector<std::string> image_paths;

    createTestImageDirectory(test_images_dir);
    for (size_t i = 0; i < number_of_images; ++i) {
        std::string image = test_images_dir + "/img_" + std::to_string(i) + ".png";
        createTestImage(image, cv::Scalar(128 + i % 128, 128, 128));
        image_paths.push_back(image);
    }

    createTestDataset(test_dataset_file, image_paths);

    std::vector<cv::Mat> images = loadDataset(test_dataset_file);

    // Verify we loaded all images
    EXPECT_EQ(images.size(), number_of_images);
}

TEST_F(ImageUtilTest, LuminanceLargeImage) {
    // Test luminance with larger images
    cv::Mat largeImage(1000, 1000, CV_8UC3, cv::Scalar(200, 150, 100));

    cv::Mat result = luminance(largeImage);

    EXPECT_EQ(result.size(), cv::Size(1000, 1000));
    EXPECT_EQ(result.channels(), 1);

    // Check a few pixels
    EXPECT_LE(result.at<uchar>(0, 0), 255);
    EXPECT_GE(result.at<uchar>(0, 0), 0);
}

TEST_F(ImageUtilTest, LuminanceNonSquareImage) {
    // Test luminance with non-square images
    cv::Mat non_square(100, 200, CV_8UC3, cv::Scalar(100, 150, 200));

    cv::Mat result = luminance(non_square);

    EXPECT_EQ(result.rows, 100);
    EXPECT_EQ(result.cols, 200);
}

TEST_F(ImageUtilTest, LuminanceVerySmallImage) {
    // Test luminance with very small images
    cv::Mat smallImage(1, 1, CV_8UC3, cv::Scalar(100, 100, 100));

    cv::Mat result = luminance(smallImage);

    EXPECT_EQ(result.rows, 1);
    EXPECT_EQ(result.cols, 1);
    EXPECT_EQ(result.channels(), 1);
    EXPECT_EQ(result.at<uchar>(0, 0), 100);
}

TEST_F(ImageUtilTest, LuminanceVeryWideImage) {
    // Test luminance with very wide images
    cv::Mat wideImage(1, 10000, CV_8UC3, cv::Scalar(50, 50, 50));

    cv::Mat result = luminance(wideImage);

    EXPECT_EQ(result.rows, 1);
    EXPECT_EQ(result.cols, 10000);
}

TEST_F(ImageUtilTest, LuminanceVeryTallImage) {
    // Test luminance with very tall images
    cv::Mat tallImage(10000, 1, CV_8UC3, cv::Scalar(50, 50, 50));

    cv::Mat result = luminance(tallImage);

    EXPECT_EQ(result.rows, 10000);
    EXPECT_EQ(result.cols, 1);
}


TEST_F(ImageUtilTest, LuminanceAirplane) {
    auto repository_root = executable_path().parent_path().parent_path(); 
    auto original_path = repository_root.string() + "/datasets/airplane/shrinking_kernel/10.png";
    auto grayscale_path = repository_root.string() + "/datasets/airplane/shrinking_kernel/grayscale.png";

    auto original = cv::imread(original_path);

    // the grayscale.png airplane image was converted with ffmpeg.
    auto expected = cv::imread(grayscale_path);
    cv::Mat one_channel_expected;
    cv::extractChannel(expected, one_channel_expected, 0);

    cv::Mat actual = luminance(original);

    // Small numerical differences
    double maxDiff = cv::norm(one_channel_expected, actual, cv::NORM_INF);
    EXPECT_LE(maxDiff, 1.0);

    // Peak Signal-to-Noise Ratio
    // Typical values:
    // 50 dB: almost identical
    // 40 – 50 dB: excellent
    // 30 – 40 dB: noticeable but small differences
    double psnr = cv::PSNR(one_channel_expected, actual);
    EXPECT_GT(psnr, 50.0);
}
