#include <functional>
#include <iostream>
#include <vector>

using namespace std;

class segment_tree {
    vector<int> tree;
    function<int(int, int)> op;  // Associative operation.

    void build(int v, int tl, int tr, vector<int> a) {
        if (tl == tr) {
            tree[v] = a[tl];
            return;
        }
        int tm = (tl + tr) / 2;
        build(2 * v, tl, tm, a);
        build(2 * v + 1, tm + 1, tr, a);
        tree[v] = op(tree[v * 2], tree[v * 2 + 1]);
    }

   public:
    segment_tree(vector<int> array, function<int(int, int)> op_) {
        op = op_;
        int n = array.size();
        tree.resize(4 * n);
        build(1, 0, n - 1, array);
    }
    // v - vertex to start from, default is 0
    // tl - left bound of tree to search from
    // tr - right bound of tree to search to
    // ql - left bound of query
    // qr - right bound of query
    int query(int tl, int tr, int ql, int qr, int v = 1) {
        if (ql > qr) {
            return 0;
        }
        if (tl == ql && tr == qr) {
            return tree[v];
        }
        int tm = (tl + tr) / 2;
        int left_res = query(tl, tm, ql, min(qr, tm), v * 2);
        int right_res = query(tm + 1, tr, max(ql, tm + 1), qr, v * 2 + 1);
        return op(left_res, right_res);
    }
};
int main() {
    int n, x;
    cin >> n;
    vector<int> initial_array;
    for (int i = 0; i < n; i++) {
        cin >> x;
        initial_array.push_back(x);
    }
    
    segment_tree tree(initial_array, [](int a, int b){return a + b;});
    int l, r;
    cin >> l >> r;
    cout << tree.query(0, n - 1, l, r) << endl;
    return 0;
}
