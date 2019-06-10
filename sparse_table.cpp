/**
 * Copyright 2019. Author Paul Kovalov
 * */

#include <iostream>
#include <vector>
#include <cmath>

using std::vector;
using std::cin;
using std::cout;
using std::min;
using std::max;
using std::endl;

template <class T> class SparseTable {
    
    // sparse table itself
    vector<vector<T > > sparse_table;

    void build_sparse_table(vector<T> arr) {
        // get height of the table
        int height = ceil(log(arr.size()) / log(2));
        // sparse table is of size height * arr.size()
        sparse_table.resize(arr.size());
        for (size_t i = 0; i < sparse_table.size(); ++i) {
            sparse_table[i].resize(height, 0);
            sparse_table[i][0] = arr[i];
        }
        int arr_s = arr.size();
        // building the table
        for (int j = 1; (1 << j) <= arr.size(); ++j) {
            for (int i = 0;(i + (1 << j) - 1) < arr.size(); ++i) {
                // sparse_table[k][i] = min(sparse_table[k-1][i], sparse_table[k-1][i + pow(2, k-1)]); 
                sparse_table[i][j] = min(sparse_table[i][j - 1], sparse_table[i + (1 << (j- 1))][j - 1]);
            }
        }
    }
    
 public:
 // accepts an array of type T, for which sparse table is built
    SparseTable(vector<T> arr) {
        build_sparse_table(arr);
    }
    int query(int l, int r) {
        int k = (int)log2(r - l + 1);
        return min(sparse_table[l][k], sparse_table[r - (1 << k) + 1][k]);
    }
};


int main() {
    // size of the input array and ariable to read array's element in
    int n, x;
    cin >> n;
    vector<int> v;

    for (int i = 0; i < n; ++i) {
        cin >> x;
        v.push_back(x);
    }
    SparseTable<int> st(v);

    cout <<  "Enter l, r" << endl;
    int l, r;
    cin >> l >> r;
    cout << st.query(l, r) << endl;
    return 0;
}