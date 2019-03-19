/**
 * Copyright 2019. Author Paul Kovalov
 * */
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::max;

class SparseTable {
    
    // sparse table itself
    vector<vector<int> > sparse_table;

    void build_sparse_table(vector<int> arr) {
        // get height of the table
        int height = ceil(log(arr.size()) / log(2));
        // sparse table is of size height * arr.size()
        sparse_table.resize(height);
        for (size_t i = 0; i < sparse_table.size(); ++i) {
            sparse_table.at(i).resize(arr.size(), 0);
        }
        // obviously st[0] is the given array
        sparse_table.at(0) = arr;
        int arr_s = arr.size();
        // building the table
        for (int k = 1; k < height; ++k) {
            for (int i = 0; i < arr_s - (1 << (k-1)); ++i) {
                sparse_table.at(k).at(i) = min(sparse_table.at(k-1).at(i), sparse_table.at(k-1).at(i + pow(2, k-1))); 
            }
        }
    }
    
 public:
 // accepts an array of type T, for which sparse table is built
    SparseTable(vector<int> arr) {
        build_sparse_table(arr);
    }
    int query(int l, int r) {
        int k = round(log(r - l + 1) / log(2));
        return min(sparse_table.at(k).at(l), sparse_table.at(k).at(r - pow(2,k) + 1));
    }
};

class RMQ {

    int BLOCK_SIZE;
    // precalculated values for each block for each l and r
    // blocks[mask][l][r] keeps precalculated RMQ from l to r
    vector<vector<vector<int> > > blocks;
    // blocks that will store subarrays for each block
    vector<vector<int> > pm1_blocks;
    vector<int> block_masks;
    // blocks amount is equal to sqrt(n);
    int BITMASKS;
    // stores minimum element for each block
    vector<int> block_mins;
    //Sparse table for block minimums
    SparseTable* st;

    void build_blocks(const vector<int> &a) {
        int blocks_q = round(double(a.size()) / double(BLOCK_SIZE));
        cout << "AMOUNT OF BLOCKS: " << blocks_q << endl;
        blocks.resize(BITMASKS);
       
        block_masks.resize(blocks_q);
        pm1_blocks.resize(blocks_q);
        block_mins.resize(blocks_q, 1e9);

        for (int k = 0; k < blocks_q; ++k) {
            pm1_blocks[k].resize(BLOCK_SIZE);
        }
        // fill in block minimums
        for (size_t k = 0; k < a.size(); ++k){
            pm1_blocks.at(k/BLOCK_SIZE).at(k%BLOCK_SIZE) = a[k];
            // also count minimum for each block
            if (block_mins[k/BLOCK_SIZE] > a[k]) {
                block_mins[k/BLOCK_SIZE] = a[k];
            }    
        }
        // precompute block masks for each block
        for (int k = 0; k < blocks_q; ++k) {
            int block_mask = 0, curr_bit = 0;
            // first element for each block
            int first_el = pm1_blocks[k][0];            
            for (int i = 1; i < BLOCK_SIZE; ++i) {
                // here I assume that +1 means 1, and -1 is 0
                // if previous value was less than current, then current bit should be 1, otherwise it is 0    
                curr_bit = (pm1_blocks[k][i-1] - first_el) < (pm1_blocks[k][i] - first_el)? 1:0; 
                // compute bitmask value for each block
                block_mask += curr_bit*(1 << (i - 1));
                cout << "BLOCK # " << k <<" ID " << block_mask << endl;
                block_masks[k] = block_mask;
            }
        }

        for (int k = 0; k < blocks_q; ++k) {
            int mask = block_masks[k];
            // if this mask wasn't computed yet - compute it, if was - just skip this block
            if (blocks[mask].empty()) {
                cout << "COMPUTING MASK " << mask << endl;
                blocks[mask].resize(BLOCK_SIZE);
                for (int i = 0; i < BLOCK_SIZE; ++i) {
                    blocks[mask][i].resize(BLOCK_SIZE);
                }
                for (int l = 0; l < BLOCK_SIZE; ++l) {
                    blocks[mask][l][l] = l;
                    for (int r = l + 1; r < BLOCK_SIZE; ++r) {
                        // now just build a lookup table 
                        blocks[mask][l][r] = pm1_blocks.at(k).at(blocks[mask][l][r - 1]) < pm1_blocks.at(k).at(r) ? blocks[mask][l][r-1]:r;
                    }
                }
                // for (int i = 0; i < BLOCK_SIZE; ++i) {
                //     for (int j = 0; j < BLOCK_SIZE; ++j) {
                //         cout << blocks[mask][i][j] << " ";
                //     }
                //     cout << endl;
                // }
            }
        }
    }

 public:
    // array to preprocess
    RMQ(const vector<int> &arr) {
        // 1/2 (log(N)), where N is the size of input array
        BLOCK_SIZE = round(0.5*(log(arr.size()) / log(2)));
        cout << "BLOCKS SIZE: " << BLOCK_SIZE << endl;
        // amount of bitmasks
        
        BITMASKS = round(0.5*sqrt(arr.size()));
        cout << "AMOUNT OF BITMASKS: " << BITMASKS << endl;
        build_blocks(arr);
        
        // create new sparse table for block minimums
        st = new SparseTable(block_mins);
    }
    int min_query(int l, int r) {
        if (l > r) {
            std::swap(l, r);
        }
        // firstly check if the result is within a 1 block
        int left_block = l / BLOCK_SIZE;
        int right_block = r / BLOCK_SIZE;
        if (left_block == right_block) {
            return pm1_blocks[left_block][blocks[block_masks[left_block]][l%BLOCK_SIZE][r%BLOCK_SIZE]];            
        }
        // to store final answer
        int answer = 0;
        // get answer for suffix
        int ans1 = blocks[block_masks[left_block]][l%BLOCK_SIZE][BLOCK_SIZE - 1];
        // get answer for prefix
        int ans2 = blocks[block_masks[right_block]][0][r%BLOCK_SIZE];
        
        answer = min(pm1_blocks[left_block][ans1], pm1_blocks[right_block][ans2]);
        // get answer in interval from sparse table
        // if there are blocks in between
        if (left_block + 1 < right_block) {
            int ans3 = st->query(left_block, right_block);
            cout << "Sparse table answer: " << ans3 << endl;
            answer = min(answer, ans3);
        } 
        return answer;
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> v(n);
   
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }
    RMQ rmq(v);
    int l, r;
    cout << "Enter query: "<< endl;
    cin >> l >> r;
    cout << rmq.min_query(l,r) << endl;
}