#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class sqrt_decompose {
    vector<int> a, b;
    int len;

   public:
    sqrt_decompose(vector<int> a) {
        this->a = a;
        int n = a.size();
        len = (int)sqrt(n) + 1;
        b.resize(len);
        for (int i = 0; i < n; ++i) {
            b[i / len] += a[i];
        }
    }

    int rsq(int l, int r) {
        int left_block = l / len;
        int right_block = r / len;
        int sum = 0;
        if (left_block == right_block) {
            for (int i = l; i <= r; ++i) {
                sum += a[i];
            }
        } else {
            for (int i = l; i < (left_block + 1) * len; ++i) {
                sum += a[i];
            }
            for (int i = left_block + 1; i < right_block; ++i) {
                sum += b[i];
            }
            for (int i = right_block * len; i <= r; ++i) {
                sum += a[i];
            }
        }
        return sum;
    }
};

int main() {
    vector<int> a;
    for (int i = 0; i < 100; i++) {
        a.push_back(i + 1);
    }
    sqrt_decompose sd(a);
    int l, r;
    while (true) {
        cout << "Enter query" << endl;
        cin >> l >> r;
        cout << sd.rsq(l, r) << endl;
    }

    return 0;
}
