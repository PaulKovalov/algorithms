#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

using namespace std;

const double PI = acos(-1);

void fast_fourier_transform(vector<complex<double>> &a, bool invert) {
    int n = (int)a.size();
    if (n == 1) return;

    vector<complex<double>> a0(n / 2), a1(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }
    fast_fourier_transform(a0, invert);
    fast_fourier_transform(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    complex<double> w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i < n / 2; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

void multiply(const vector<int> &a, const vector<int> &b, vector<int> &res) {
    // two polynoms to store the dft result of the substitution of th roots of 1
    vector<complex<double>> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int a_size_log = ceil(log(a.size())/log(2)); // the power of 2 that is bigger then the size of the a
    int b_size_log = ceil(log(b.size())/log(2)); // the power of 2 that is bigger then the size of the b
    int n = max(pow(2, a_size_log + 1),pow(2, b_size_log + 1)); // new required size
    int old_a_size = a.size();
    int old_b_size = b.size();
    
    fa.resize(n);
    fb.resize(n);    

    bool direct_transform = false, inverse_transform = true;
    fast_fourier_transform(fa, direct_transform);
    fast_fourier_transform(fb, direct_transform);
    for (size_t i = 0; i < n; ++i) {
        fa[i] *= fb[i];
    }
    fast_fourier_transform(fa, inverse_transform);
    res.resize(n);
    for (size_t i = 0; i < n; ++i) {
        res[i] = int(fa[i].real() + 0.5);
    }
}


int main() {
    int a_n = 0, b_n = 0;
    vector<int> a, b;
    cout << "Enter number of the first polynom coefficients: ";
    cin >> a_n;
    a.resize(a_n);
    for (int i = 0; i < a_n; ++i) {
        cout << "Enter the " << i << "'th coefficient: ";
        cin >> a[i];
    }
    cout << "Enter number of the second polynom coefficients: ";
    cin >> b_n;
    b.resize(b_n);
    for (int i = 0; i < b_n; ++i) {
        cout << "Enter the " << i << "'th coefficient: ";
        cin >> b[i];
    }    
    vector<int> res;
    multiply(a, b, res);
    for (int i = 0; i < res.size(); ++i) {
        if (res[i]) {
            cout << res[i] << "x^" << i << (i + 1 == res.size() || !res[i + 1] ? "" :" + ");
        }
    }
    cout << endl;
    return 0;
}