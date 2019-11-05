#include "fourier-analysis/WindowFunctions.hpp"
#include <vector>
#include <cmath>

typedef vector<double> Window;

using namespace cv;
using namespace std;

void print(vector<double> const &input) {
    for (auto const& i: input) {
        cout << i << endl;
    }
}

int main() {
    const int n = 5;
    WindowFunctions *windowFunctions = new WindowFunctions();
    Window window = windowFunctions->generate(n, "flattop");
    print(window);
    return 0;
}