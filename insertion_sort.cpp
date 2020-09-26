#include <iostream>
#include <vector>

using namespace std;

int main() {
    srand(time(NULL));
    int n = 1000;
    vector<int> v(n);
    for (int i = 0; i < 1000; ++i) {
        v[i] = rand() % n;
    }
    // insertion sort

    for (int j = 1; j < n; ++j) {
        int key = v[j];
        int i = j - 1;
        // changing ">" to "<" results in descending sorting
        while (i >= 0 && v[i] > key) {
            v[i + 1] = v[i];
            --i;
        }
        v[i + 1] = key;
    }

    // assert that v is sorted
    for (int i = 0; i < n - 1; ++i) {
        if (v[i] < v[i + 1]) {
            cout << "not sorted " << endl;
            return 0;
        }
    }
    cout << "sorted" << endl;
    return 0;
}
