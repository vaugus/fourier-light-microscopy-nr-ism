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
    std::string testImagesDir;
    std::string testDatasetFile;

    void SetUp() override
    {
        testImagesDir = "/tmp/ftiqa_test_images";
        testDatasetFile = "/tmp/ftiqa_test_images.txt";

        // Clean up any existing directories
        if (std::filesystem::exists(testImagesDir))
        {
            std::filesystem::remove_all(testImagesDir);
        }
    }

    void TearDown() override
    {
        // Clean up
        if (std::filesystem::exists(testImagesDir))
        {
            std::filesystem::remove_all(testImagesDir);
        }
        if (std::filesystem::exists(testDatasetFile))
        {
            std::filesystem::remove(testDatasetFile);
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

static void createTestImageDirectory(const std::string &testImageDirectory) 
{
    if (!std::filesystem::exists(testImageDirectory))
    {
        std::filesystem::create_directory(testImageDirectory);
    }
}

// Helper function to create simple test images
static void createTestImage(const std::string &filename, cv::Scalar color)
{
    cv::Mat image(100, 100, CV_8UC3, color);
    cv::imwrite(filename, image);
}

// Helper function to create test dataset file
static void createTestDataset(const std::string &datasetFile, const std::vector<std::string> &imagePaths)
{
    std::ofstream file(datasetFile);
    for (const auto &path : imagePaths)
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
    double pixelValue = gray.at<uchar>(50, 50);
    // Should be in valid range
    EXPECT_LE(pixelValue, 255);
    EXPECT_GE(pixelValue, 0);
}

TEST_F(ImageUtilTest, LuminanceConversionWhiteImage)
{
    // Create a test BGR image - pure white
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(255, 255, 255));

    // Convert to grayscale
    cv::Mat gray = luminance(image);

    // White should have maximum luminance
    double pixelValue = gray.at<uchar>(0, 0);
    EXPECT_EQ(pixelValue, 255);
}

TEST_F(ImageUtilTest, LuminanceConversionBlackImage)
{
    // Create a test BGR image - pure black
    cv::Mat image(100, 100, CV_8UC3, cv::Scalar(0, 0, 0));

    // Convert to grayscale
    cv::Mat gray = luminance(image);

    // Black should have zero luminance
    double pixelValue = gray.at<uchar>(0, 0);
    EXPECT_EQ(pixelValue, 0);
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
    testImagesDir = "/tmp/ftiqa_test_images2";
    testDatasetFile = "/tmp/ftiqa_test_images2.txt";

    // Create test images
    std::string image1 = testImagesDir + "/image1.png";
    std::string image2 = testImagesDir + "/image2.png";
    std::string image3 = testImagesDir + "/image3.png";

    createTestImageDirectory(testImagesDir);

    createTestImage(image1, cv::Scalar(255, 0, 0));
    createTestImage(image2, cv::Scalar(0, 255, 0));
    createTestImage(image3, cv::Scalar(0, 0, 255));

    // Create dataset file
    createTestDataset(testDatasetFile, {image1, image2, image3});

    // Load dataset
    std::vector<cv::Mat> images = loadDataset(testDatasetFile);

    // Verify we loaded all images
    EXPECT_EQ(images.size(), 3u);

    // Verify image dimensions
    EXPECT_EQ(images[0].size(), cv::Size(100, 100));
    EXPECT_EQ(images[1].size(), cv::Size(100, 100));
    EXPECT_EQ(images[2].size(), cv::Size(100, 100));
}

TEST_F(ImageUtilTest, LoadDatasetWithSingleImage) {
    // Create single image test
    testImagesDir = "/tmp/ftiqa_test_images3";
    testDatasetFile = "/tmp/ftiqa_test_images3.txt";

    createTestImageDirectory(testImagesDir);

    std::string singleImage = testImagesDir + "/single.png";
    createTestImage(singleImage, cv::Scalar(128, 128, 128));

    createTestDataset(testDatasetFile, {singleImage});

    std::vector<cv::Mat> images = loadDataset(testDatasetFile);

    EXPECT_EQ(images.size(), 1u);
    EXPECT_EQ(images[0].size(), cv::Size(100, 100));
}

TEST_F(ImageUtilTest, LoadDatasetWithEmptyFile) {
    // Test with empty dataset file
    testDatasetFile = "/tmp/ftiqa_test_images_empty.txt";
    std::ofstream file(testDatasetFile);
    // Don't write anything, leave empty

    std::vector<cv::Mat> images = loadDataset(testDatasetFile);

    // Empty file should load empty vector
    EXPECT_EQ(images.size(), 0u);
}

TEST_F(ImageUtilTest, LoadDatasetWithNonExistentFile) {
    // Test with non-existent file
    testDatasetFile = "/nonexistent/path/to/images.txt";

    EXPECT_THROW(loadDataset(testDatasetFile), std::runtime_error);
}

TEST_F(ImageUtilTest, LoadDatasetWithInvalidImagePath) {
    // Create test file with one invalid image path
    testImagesDir = "/tmp/ftiqa_test_images4";
    testDatasetFile = "/tmp/ftiqa_test_images4.txt";

    createTestImageDirectory(testImagesDir);

    // Create only one image
    std::string validImage = testImagesDir + "/valid.png";
    createTestImage(validImage, cv::Scalar(128, 128, 128));

    // Add invalid path
    createTestDataset(testDatasetFile, {validImage, "/nonexistent/image.png"});

    auto oldLevel = getLogLevel();
    setLogLevel(LOG_LEVEL_SILENT);

    // Should throw exception
    EXPECT_THROW(loadDataset(testDatasetFile), std::runtime_error);

    setLogLevel(oldLevel);
}

TEST_F(ImageUtilTest, LoadDatasetWithEmptyLines) {
    // Test dataset file with empty lines at end
    testImagesDir = "/tmp/ftiqa_test_images5";
    testDatasetFile = "/tmp/ftiqa_test_images5.txt";

    std::string image1 = testImagesDir + "/image1.png";
    createTestImageDirectory(testImagesDir);
    createTestImage(image1, cv::Scalar(128, 128, 128));

    std::ofstream file(testDatasetFile);
    file << image1 << "\n";
    file << "\n";  // Empty line
    file << "\n";  // Another empty line
    file.close();

    auto oldLevel = getLogLevel();
    setLogLevel(LOG_LEVEL_SILENT);
    
    // Should throw exception
    EXPECT_THROW(loadDataset(testDatasetFile), std::runtime_error);

    setLogLevel(oldLevel);
}

TEST_F(ImageUtilTest, LoadDatasetLargeDataset) {
    // Create a larger dataset to test performance and handling
    testImagesDir = "/tmp/ftiqa_test_images6";
    testDatasetFile = "/tmp/ftiqa_test_images6.txt";

    const size_t numberOfImage = 5000;
    std::vector<std::string> imagePaths;

    createTestImageDirectory(testImagesDir);
    for (size_t i = 0; i < numberOfImage; ++i) {
        std::string image = testImagesDir + "/img_" + std::to_string(i) + ".png";
        createTestImage(image, cv::Scalar(128 + i % 128, 128, 128));
        imagePaths.push_back(image);
    }

    createTestDataset(testDatasetFile, imagePaths);

    std::vector<cv::Mat> images = loadDataset(testDatasetFile);

    // Verify we loaded all images
    EXPECT_EQ(images.size(), numberOfImage);
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
    cv::Mat nonSquare(100, 200, CV_8UC3, cv::Scalar(100, 150, 200));

    cv::Mat result = luminance(nonSquare);

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
    auto repositoryRoot = executable_path().parent_path().parent_path(); 
    auto originalPath = repositoryRoot.string() + "/datasets/airplane/shrinking_kernel/10.png";
    auto grayscalePath = repositoryRoot.string() + "/datasets/airplane/shrinking_kernel/grayscale.png";

    auto original = cv::imread(originalPath);

    // the grayscale.png airplane image was converted with ffmpeg.
    auto expected = cv::imread(grayscalePath);
    cv::Mat oneChannelExpected;
    cv::extractChannel(expected, oneChannelExpected, 0);

    cv::Mat actual = luminance(original);

    // Small numerical differences
    double maxDiff = cv::norm(oneChannelExpected, actual, cv::NORM_INF);
    EXPECT_LE(maxDiff, 1.0);

    // Peak Signal-to-Noise Ratio
    // Typical values:
    // 50 dB: almost identical
    // 40 – 50 dB: excellent
    // 30 – 40 dB: noticeable but small differences
    double psnr = cv::PSNR(oneChannelExpected, actual);
    EXPECT_GT(psnr, 50.0);
}
