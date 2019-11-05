//To read data from a text file. 
//filename is the name of the text file
//rows and cols show dimensions of the matrix written in the text file

#include "../lib/include/fft.hpp"
#include "../lib/include/fft_utils.hpp"
#include "../lib/include/helper.hpp"
#include <fstream>

using namespace std;
using namespace cv;

int main() {

    double arr[5][5] = {    {0.0000,0.0000,0.0000,0.0000,0.0000},
                            {0.2500,0.2500,0.2500,0.2500,0.2500},
                            {0.5000,0.5000,0.5000,0.5000,0.5000},
                            {0.7500,0.7500,0.7500,0.7500,0.7500},
                            {1.0000,1.0000,1.0000,1.0000,1.0000}};
                        
    Mat A = Mat(5, 5, CV_64FC1, &arr);

    FFTUtils *fft_utils = new FFTUtils();

    fft_utils->fft2(A);

    imshow("teste", A);
    waitKey(0);

    return 0;
}