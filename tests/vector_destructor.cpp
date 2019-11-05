#include <vector>
#include <iostream>

using namespace std;

int main() {

    vector<int> nums;

    for (int i = 0; i < 100; i++) {
        nums.push_back(i);
    }

    for (int i = 0; i < 100; i++) {
        cout << nums[i] << endl;
    }

    return 0;
}