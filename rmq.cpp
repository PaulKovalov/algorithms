#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <algorithm>
#include <iterator>

using std::ostream_iterator;
using std::copy;
using std::vector;
using std::cin;
using std::cout;
using std::min;
using std::endl;

class RMQ {
    class Tree {
        vector<int> first, euler_tour, heights;
        struct Node {
            Node *left, *right, *parent;
            int key, priority;
            Node(int k, int pr, Node *l, Node *r, Node *p = 0) {
                left = l;
                right = r;
                key = k;
                priority = pr;
                parent = p;
            };
        };
    
        void dfs(Node *n, int current_time) {
            euler_tour.push_back(n->key);
            heights.push_back(current_time);
            if (first[n->key] == -1) 
                first[n->key] = euler_tour.size() - 1;
            if (n->left != nullptr) {
                dfs(n->left, current_time + 1);
                euler_tour.push_back(n->key);
                heights.push_back(current_time);                    
            }
            if (n->right != nullptr) {
                dfs(n->right, current_time + 1);
                euler_tour.push_back(n->key);
                heights.push_back(current_time);                    
            }
        }
        Node *head;
       
        
     public:
        
         void build_tree(const vector<int> &v) {
            Node *last = new Node(0,v[0] ,nullptr, nullptr);
            for (size_t i = 1; i < v.size(); i++) {
                Node *new_node = new Node(i,v[i],nullptr, nullptr);
                if (new_node->priority >= last->priority) {
                    last->right = new Node(new_node->key, new_node->priority,nullptr, nullptr, last);
                    last = last->right;
                } else {
                    Node* current = last;
                    while (current->parent != nullptr && new_node->priority < current->priority) {
                        current = current->parent;
                    }
                    if (new_node->priority < current->priority) {
                        last = new Node(new_node->key, new_node->priority, current, nullptr);
                        current->parent = last;
                    } else {
                        current->right = new Node(new_node->key, new_node->priority, current->right, nullptr, current);
                        current->right->left->parent = current->right;
                        last = current->right;
                    }
                }
            }
            while (last->parent != nullptr) 
                last = last->parent;
            head = last;
        }
  
        void dfs() {
            dfs(head, 0);
        }
        vector<int> get_euler() {
            return euler_tour;
        }
        vector<int> get_heights() {
            return heights;
        }
        vector<int> get_first() {
            return first;
        }

        Tree(int n) {first.resize(n, -1);}
    };
    vector<vector<int > > sparse_table;

    void build_sparse_table(vector<int> arr) {    
        int height = ceil(log(arr.size()) / log(2));
        sparse_table.resize(arr.size());
        for (size_t i = 0; i < sparse_table.size(); ++i) {
            sparse_table[i].resize(height, 0);
            sparse_table[i][0] = arr[i];
        }
        int arr_s = arr.size();
        for (int j = 1; (1 << j) <= arr.size(); ++j) {
            for (int i = 0;(i + (1 << j) - 1) < arr.size(); ++i) {
                sparse_table[i][j] = min_by_height(sparse_table[i][j - 1], sparse_table[i + (1 << (j- 1))][j - 1]);
            }
        }
    }
    int sparse_table_query(int l, int r) {
        int k = (int)log2(r - l + 1);
        return min_by_height(sparse_table[l][k], sparse_table[r - (1 << k) + 1][k]);
    }
    

    vector<vector<vector<int> > > blocks;
    vector<int> block_masks, euler_tour, first, heights, block_mins,initial_array;
    int BLOCK_SIZE;
    int BLOCKS_AMOUNT;
    
    int min_by_height(int i, int j) {
        return heights[i] < heights[j] ? i:j;
    }
    void build_blocks(vector<int> &v) {
        
        block_mins.resize(BLOCKS_AMOUNT, 1e9);
        block_masks.resize(BLOCKS_AMOUNT, 0);
        size_t sz = v.size();
    
        for (size_t i = 0; i < sz; ++i) {
            if (block_mins[i/BLOCK_SIZE] > v[i]) {
                block_mins[i/BLOCK_SIZE] = v[i];
            }
        }
        cout << "Array passed to sparse table" << endl;
        for (int i = 0; i < block_mins.size(); ++i) {
            cout << block_mins[i] << " ";
        }
        cout << endl;
        build_sparse_table(block_mins);
        int inc = 0;

        for (int k = 0; k < BLOCKS_AMOUNT; ++k) {
            int block_mask = 0, curr_bit = 0;
            // first element for each block
            int first_el = v[k*BLOCK_SIZE];            
            for (int i = 1; i < BLOCK_SIZE; ++i) {
                // here I assume that +1 means 1, and -1 is 0
                // if previous value was less than current, then current bit should be 1, otherwise it is 0    
                curr_bit = (v[k*BLOCK_SIZE + i-1] - first_el) < (v[k*BLOCK_SIZE + i] - first_el)? 1:0; 
                // compute bitmask value for each block
                block_mask += curr_bit*(1 << (i - 1));
                cout << "BLOCK # " << k <<" ID " << block_mask << endl;
                block_masks[k] = block_mask;
            }
        }
        blocks.resize(sqrt(v.size()));
        size_t es = euler_tour.size();
        for (int i = 0; i < BLOCKS_AMOUNT; ++i) {
            int mask = block_masks[i];
            if (blocks[mask].empty()) {
                blocks[mask].assign(BLOCK_SIZE, vector<int>(BLOCK_SIZE));
                for (int l = 0; l < BLOCK_SIZE; ++l) {
                    blocks[mask][l][l] = l;
                    for (int r = l + 1; r < BLOCK_SIZE; ++r) { 
                        blocks[mask][l][r] = heights[blocks[mask][l][r - 1] + i * BLOCK_SIZE] < heights[i*BLOCK_SIZE + r] ? blocks[mask][l][r-1]:r;
                    }
                }
            }
        }
        // show blocks
        for (int i = 0; i < blocks.size(); ++i) {
            cout << "BLOCK #"<<i <<", MASK " << block_masks[i] << endl; 
            for (int j = 0; j < blocks[i].size(); ++j) {
                for (int k = 0; k < blocks[i][j].size(); ++k) {
                    cout << blocks[i][j][k] << " ";
                }
                cout << endl;
            }
        }
    }
    
 public:
    RMQ(vector<int> v){
        initial_array = v;
        Tree tree(v.size());
        tree.build_tree(v);
        tree.dfs();
        cout << "Euler tour: " << endl;
        euler_tour = tree.get_euler();
        copy(euler_tour.begin(), euler_tour.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "Firsts: " << endl;
        first = tree.get_first();
        copy(first.begin(), first.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "Heights: " << endl;
        heights = tree.get_heights();
        copy(heights.begin(), heights.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        BLOCK_SIZE = 0.5 * log2(heights.size());
        BLOCKS_AMOUNT = heights.size() / BLOCK_SIZE;
        cout << "BLOCK SIZE: " << BLOCK_SIZE << endl;
        cout << "BLOCKS AMOUNT: " << BLOCKS_AMOUNT << endl;
        build_blocks(heights);
    }
    int query(int ll, int rr) {
        
        int l, r;
        cout << "Firsts: " << endl;
        copy(first.begin(), first.end(), ostream_iterator<int>(cout, " "));
        l = first[ll];
        r = first[rr];
        cout << "Query left: "<<l<<", Query right: "<<r<<endl;
        int left_block, right_block;
        left_block = l/BLOCK_SIZE;
        right_block = r/BLOCK_SIZE;
        cout << "Left block: " << left_block << endl;
        cout << "Right block: " << right_block << endl;
        if (left_block == right_block) {
            return euler_tour.at(blocks[block_masks[left_block]][l%BLOCK_SIZE][r%BLOCK_SIZE] + left_block * BLOCK_SIZE);
        }
        int ans1 = blocks.at(block_masks.at(left_block)).at(l%BLOCK_SIZE).at(BLOCK_SIZE - 1) + left_block * BLOCK_SIZE;
        int ans2 = blocks.at(block_masks.at(right_block)).at(0).at(r%BLOCK_SIZE) + right_block * BLOCK_SIZE;
        cout << "Ans1: " <<ans1 <<  ", Ans2: " <<ans2 << endl; 
        int ans = min_by_height(ans1, ans2);
        if (left_block + 1 < right_block) {
            int ans3 = sparse_table_query(left_block + 1, right_block);
            cout << "Sparse table answer: " << ans3 << endl;
            ans = min_by_height(ans, ans3);
        }
        cout << "ANSWER: " << endl;
        cout << initial_array[euler_tour.at(ans)] << endl;
        return initial_array[euler_tour.at(ans)];
        // return 0;
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
    cout << "Enter query: " << endl;
    int l, r;
    cin >> l >> r;
    rmq.query(l,r);
    return 0;
}