#include <iostream>
#include <vector>
#include <queue>
using namespace std;
/**
 * Binary tree based implementation of SegmentTree
 * */
class SegmentTree {
    struct Node {
        Node *left, *right;
        int data;
    };
    
    int size;
    Node *head;

    void build_tree(Node *node, int tl, int tr, vector<int> &array) {
        if (tl + 1 == tr) {
            node->data = array[tl];
            return;
        }
        
        node->left = new Node();
        node->right = new Node();

        int tm = tl + (tr - tl) / 2;
        
        build_tree(node->left, tl, tm, array);
        build_tree(node->right, tm, tr, array);
        node->data = node->left->data + node->right->data;
    }
    
    int query_(int l, int r, int tl, int tr, Node *n) {
        if (l >= tr || r <= tl) {
            return 0;
        }
        if (tl == l && tr == r) {
            return n->data;
        }
        int tm = tl + (tr - tl) / 2;
        int left_res = query_(l, min(tm, r), tl, tm, n->left);
        int right_res = query_(max(l, tm), r,tm, tr, n->right);
        return left_res + right_res;
    }
  public:
    // for testing purposes only
    void print_tree() {
        queue<Node*> qn;
        qn.push(head);
        while (!qn.empty()) {
            cout << qn.front()->data <<" ";
            if (qn.front()->left != nullptr) {
                qn.push(qn.front()-> left);
            }
            if (qn.front()->right != nullptr) {
                qn.push(qn.front()-> right);
            }
            qn.pop();
        }
    }
    // constructor accepts array to build a tree from
    SegmentTree(vector<int> &array) {
        head = new Node();
        size = array.size();
        build_tree(head, 0, size, array);
    }
    int query(int l, int r) {
        return query_(l, r, 0, size, head);
    }
};
int main() {

    cout << "Enter size of an array: " << endl;
    int n;
    cin >> n;
    vector<int> v;
    cout <<"Enter array: " << endl;
    int x;
    
    for (int i = 0; i < n; i++) {
        cin >> x;
        v.push_back(x);
    }
    SegmentTree tree(v);
    cout << "Tree was built: " << endl;
    tree.print_tree();

    cout << "Enter query (l r): " << endl;
    int l, r;
    cin >> l >> r;
    cout << tree.query(l, r) << endl;
    return 0;
}