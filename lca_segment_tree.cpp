/**
 * Copyright 2019. Author Paul Kovalov
 * */

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using std::cout;
using std::cin;
using std::vector;
using std::min;
using std::max;
using std::swap;
using std::endl;

class LCA {
    vector<vector<int> > adj_list;
    vector<int> euler, heights, first;
    vector<int> times;
    vector<int> tree;
    int total_vertices = 0;
    void dfs(int v, int current_height) {
        // add current vertex to euler path
        euler.push_back(v);
        // add first entry of the vertex
        if (first[v] == -1) {
            first[v] = euler.size() - 1;
        }
        heights.push_back(current_height);
        for (size_t i = 0; i < adj_list[v].size(); i++) {
            dfs(adj_list[v][i], current_height + 1);
            euler.push_back(v);
            heights.push_back(current_height);
        }
    }
    void build_segment_tree(int v, int tl, int tr) {
        // if segment's length = 0, this segment is already built
        if (tl + 1 == tr) {
            tree[v] = euler[tl];
            return;
        }

        int tm = (tr + tl) / 2;
        build_segment_tree(2 * v + 1, tl, tm);
        build_segment_tree(2 * v + 2, tm, tr);
        tree[v] = heights[first[tree[2*v + 1]]] \
        < heights[first[tree[2*v+ 2]]] ? tree[2*v + 1] : tree[2*v + 2];
    }
    int query_(int tl, int tr, int ql, int qr, int v = 0) {
        if (ql >= qr) {
            return 1e9;
        }
        if (tl == ql && tr == qr) {
            return tree[v];
        }
        int tm = (tr + tl) / 2;
        int left_res = query_(tl, tm, ql, min(qr, tm), v * 2 + 1);
        int right_res = query_(tm, tr, max(tm, ql), qr, v * 2 + 2);
        if (left_res == 1e9) {
            return right_res;
        } else if (right_res == 1e9) {
            return left_res;
        }
        return heights[first[left_res]] < \
        heights[first[right_res]] ? left_res:right_res;
    }

 public:
    // constructor takes adjacency list of the tree
    explicit LCA(vector<vector<int> > list): adj_list(list) {
        // calculate total amount of vertices in a graph
        total_vertices = adj_list.size();
        first.resize(total_vertices, -1);
        // start preprocessing from dfs
        dfs(0, 0);
        // build segment tree
        int n = heights.size();
        tree.resize(4*n);
        build_segment_tree(0, 0, heights.size());
    }
    int query(int ql, int qr) {
        int left = first[ql];
        int right = first[qr];
        if (right < left)
            swap(left, right);
        return query_(0, euler.size(), left, right);
    }
};
int main() {
    int num_of_vertices, num_of_curr, xx;
    cin >> num_of_vertices;
    vector<vector<int> > list;
    for (int i = 0; i < num_of_vertices; i++) {
        cin >> num_of_curr;
        vector<int> v;
        for (int j = 0; j < num_of_curr; j++) {
            cin >> xx;
            v.push_back(xx);
        }
        list.push_back(v);
    }
    LCA lca(list);
    int v1, v2;
    cin >> v1 >> v2;
    cout << lca.query(v1, v2) << endl;
    return 0;
}
