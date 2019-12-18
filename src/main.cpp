#include "../lib/include/sample_function.hpp"
#include "../lib/include/fft_utils.hpp"
#include "../lib/include/helper.hpp"
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
    std::vector<cv::Mat> img_vec;
    std::string const path = argv[1];

    SampleFunction *sample_function = new SampleFunction();
    Helper *helper = new Helper();

    helper->open_dataset(img_vec, path);

    sample_function->run_batch_fft(img_vec);

    delete sample_function;
    delete helper;
    img_vec.clear();

    return 0; 
}


// // Function to give index of the median 
// int median(int* a, int l, int r) 
// { 
//     int n = r - l + 1; 
//     n = (n + 1) / 2 - 1; 
//     return n + l; 
// } 
  
// // Function to calculate IQR 
// int IQR(int* a, int n) 
// { 
//     sort(a, a + n); 
  
//     // Index of median of entire data 
//     int mid_index = median(a, 0, n); 
  
//     // Median of first half 
//     int Q1 = a[median(a, 0, mid_index)]; 
  
//     // Median of second half 
//     int Q3 = a[median(a, mid_index + 1, n)]; 
  
//     // IQR calculation 
//     return (Q3 - Q1); 
// }

// // Driver Function 
// int main() 
// { 
//     int a[] = {1,2,3,4,5,6,7,8, 9}; 
//     int n = sizeof(a)/sizeof(a[0]); 
//     cout << IQR(a, n) << endl; 
//     return 0; 
// } 