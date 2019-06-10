#include <vector>
#include <iostream>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

class ImplicitTreap {
    struct Node {
        int size;
        int priority;
        Node *left, *right;
    };
    Node *head;
    void merge(Node *root, Node *&l, Node *&r) {
        if (l == nullptr) {
            root = r;
            return;
        }
        if (r == nullptr) {
            root = l;
            return;
        }

        if (r->priority > l->priority) {
            root = r;
            merge(root->left, l, r->left);
        } else {
            root = l;
            merge(root->right, l->right, r);
        }
    }
    void split(Node *root, int key, Node *&l, Node *&r) {
        // ImplicitTreap newTree = null;
        // int curIndex = SizeOf(Left) + 1;

        // if (curIndex <= x)
        //     {
        //     if (Right == null)
        //         R = null;
        //     else
        //         Right.Split(x - curIndex, out newTree, out R);
        //     L = new ImplicitTreap(y, Cost, Left, newTree);
        //     L.Recalc();
        // }
        // else
        // {   
        //     if (Left == null)
        //         L = null;
        //     else
        //         Left.Split(x, out L, out newTree);
        //     R = new ImplicitTreap(y, Cost, newTree, Right);
        //     R.Recalc();
        // }
    }
    int size_of(Node *n) {
        return n == nullptr ? 0: n->size;
    }
    int recalc(Node *n) {
        return size_of(n->left) + size_of(n->right) + 1;
    }
 public:
    ImplicitTreap() {
        head = nullptr;
    }
    
}