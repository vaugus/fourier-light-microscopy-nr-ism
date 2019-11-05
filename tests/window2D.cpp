#include <iostream>
#include <numeric>
#include <vector>
#include "../lib/fourier-analysis/window_functions.hpp"

using namespace std;

int main() {
	WindowFunctions *wf = new WindowFunctions();
	wf->generate_2D_window(10, 5, "hann");

    return 0;
}

/**
 *
 *
 *
 */