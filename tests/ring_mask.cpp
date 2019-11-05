#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "../lib/fourier-analysis/fourier_analysis.hpp"

using namespace std;
using namespace cv;
 
int main() {

    FourierAnalysis *fa = new FourierAnalysis();
    Mat mid = fa->generate_ring_mask(512, 512, "mid");    
    Mat high = fa->generate_ring_mask(512, 512, "high");    
    Mat highest = fa->generate_ring_mask(512, 512, "highest");    

    imshow("mid", mid);
    imshow("high", high);
    imshow("highest", highest);

    waitKey(0);

    return(0); 
}