/**
 * Copyright 2019, Author - Paul Kovalov
 * */

#include <iostream>
#include <vector>

using namespace std;
/**
 * Array based implementation of a segment tree
 * */
class SegmentTree {
    vector<int> tree;
    // builds a tree by given array a, the operation used is "sum"
    void build(int v, int tl, int tr, vector<int> a) {
        // if segment's length = 0, this segment is already built
        if (tl + 1 == tr) {
            tree[v] = a[tl];
            return;
        }
        int tm = (tr + tl) / 2;
        build(2 * v + 1, tl, tm, a);
        build(2 * v + 2, tm, tr, a);
        tree[v] = min(tree[v * 2 + 1], tree[v * 2 + 2]);
    }
 public:
    // constructor accepts array of integers
    SegmentTree(vector<int> array) {
        int n = array.size();
        tree.resize(4*n);
        build(0, 0, n,array);
        for (int i = 0; i < tree.size(); i++) {
            cout << tree[i] <<" ";
        }
        cout <<endl;
    }
    // v - vertex to start from, default is 0
    // tl - left bound of tree to search from
    // tr - right bound of tree to search to
    // ql - left bound of query
    // qr - right bound of query
    int query(int tl, int tr, int ql, int qr, int v = 0) {
        if (ql >= qr) {
            return 1e9;
        }
        if (tl == ql && tr == qr) {
            return tree[v];
        }
        int tm = (tr + tl) / 2;
        int left_res = query(tl, tm, ql, min(qr, tm),v * 2 + 1);
        int right_res = query(tm, tr, max(tm, ql), qr,v * 2 + 2);
        return min(left_res, right_res);
    }
};
int main() {
    int n, x;
    cin >> n;
    vector <int> initial_array;
    for (int i = 0; i < n; i++) {
        cin >> x;
        initial_array.push_back(x);
    }
    cout << "Initial array: " << endl;
    for (int i = 0; i < initial_array.size(); i++) {
        cout << initial_array[i] <<" ";
    }
    SegmentTree tree(initial_array);
    int l, r;
    cin >> l >> r;
    cout << tree.query(0, n, l, r) << endl;
    return 0;
}