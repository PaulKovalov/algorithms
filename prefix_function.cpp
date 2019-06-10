#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    string str;
    cin >> str;
    vector<int> pi(str.size(), 0);
    // prefix function
    int n = str.size(), k;
    for (int i = 1; i < n; ++i) {
        k = pi[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = pi[k - 1];
        }
        if (str[i] == str[k]) {
            k += 1;
        }
        pi[i] = k;
    }
    for (int i = 0; i < n; ++i) {
        cout << pi[i] << " ";
    }
    cout << endl;
    return 0;
}