/*  Snippet code for Operations with images tutorial (not intended to be run but should built successfully) */

// #include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "../lib/fourier-analysis/fourier_analysis.cpp"

using namespace cv;
using namespace std;

int main(int,char**)
{
    std::string filename = "/home/victor/Documents/cpp-msc-workspace/blur-analysis/airplane.png";
    // Input/Output
    //! [Load an image from a file]
    Mat img = imread(filename);

    //! [BGR to Gray]
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2HSV);
    //! [BGR to Gray]

    imshow("gray", gray);
    waitKey();

    {
        std::string filename = "/home/victor/Documents/cpp-msc-workspace/blur-analysis/airplane.png";
        // Input/Output
        //! [Load an image from a file]
        Mat img = imread(filename);

        //! [BGR to Gray]
        FourierAnalysis *fa = new FourierAnalysis();

        Mat gray = fa->convert_colourspace(img, "grayscale");
        imshow("image", gray);

        waitKey();

        return 0;
    }

    return 0;
}