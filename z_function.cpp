#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    string str;
    cin >> str;
    vector<int> z(str.size(), 0);
    // z function
    int left = 0, right = 0;
    int n = str.size();
    for (int i = 1; i < n; ++i) {
        if (i <= right) {
            z[i] = min(z[i - left], right - i + 1);
        } 
        while ((i + z[i] < n) && (str[z[i]]  == str[i + z[i]])) {
            ++z[i];
        }
        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << z[i] << " ";
    }
    cout << endl;
    return 0;
}