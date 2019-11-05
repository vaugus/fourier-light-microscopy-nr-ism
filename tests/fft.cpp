int main()
{
    clock_t tStart = clock();


    int *z = teste();
    cout << z[1] << endl;
    Mat M = Mat::zeros(4,4, CV_32F);
    cout << "M = "<< endl << " "  << M << endl << endl;

    float elem_a = M.at<float>(3,3);
    cout << elem_a << endl;

    const char* filename = "/home/victor/Documents/cpp-msc-workspace/blur-analysis/airplane.png";
    // const char* filename = "/home/victor/Documents/msc-image-database/card/1.tif";


    Mat I = imread(filename, IMREAD_GRAYSCALE);
    if( I.empty()){
        cout << "Error opening image" << endl;
        return -1;
    }

    Mat padded;
  
    const int padSize = max(I.rows, I.cols);

    copyMakeBorder(I, padded, 0, padSize - I.rows, 0, padSize - I.cols, BORDER_CONSTANT, Scalar::all(0));
    
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
    dft(complexI, complexI);            // this way the result may fit in the source matrix
   
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
   
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
   
    Mat magI = planes[0];
   
    magI += Scalar::all(1);                    // switch to logarithmic scale
   
    log(magI, magI);
   
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
   
    magI = fftShift(magI);
    
    imshow("Input Image"       , I   );    // Show the result
    imshow("spectrum magnitude", magI);
    
    waitKey();
    /* Do your stuff here */
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}