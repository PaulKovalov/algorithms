/**
 * Copyright 2019. Author Paul Kovalov
 * */
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::max;

class AVL_Tree {
    struct Node {
        int value;
        Node *left, *right;
        int height = -1;
    };
    Node *head;
    // returns node height, checks if node != nullptr. If so, returns -1
    int get_node_height(Node *n) {
        if (n == nullptr) {
            return -1;
        }
        return n->height;
    }
    void left_rotate(Node *&root) {
        Node* y = root->right;
        Node* x = y->left;
        y->left = root;
        root->right = x;
        y->height = max(get_node_height(y->left), get_node_height(y->right)) + 1;
        root->height = max(get_node_height(root->left), get_node_height(root->right)) + 1;
        root = y;
    }
    void right_rotate(Node *&root) {
        Node *x = root->left;
        Node *y = x->right;
        x->right = root;
        root->left = y;
        root->height = max(get_node_height(root->left), get_node_height(root->right)) + 1;
        x->height = max(get_node_height(x->left), get_node_height(x->right)) + 1;
        root = x;
    }
    void add_(Node *&root, int value) {
        if (root == nullptr) {
            root = new Node();
            root->value = value;
        } else if (value > root->value) {
            add_(root->right, value);
        } else if (value < root->value) {
            add_(root->left, value);
        }
        root->height = max(get_node_height(root->left), get_node_height(root->right)) + 1;
        int balance = get_node_height(root->left) - get_node_height(root->right);

        // left left case
        if (balance > 1 && value < root->left->value) {
            right_rotate(root);
        }
        // right right case
        if (balance < -1 && value > root->right->value) {
            left_rotate(root);
        }
        // left right case
        if (balance > 1 && value > root->left->value) {
            left_rotate(root->left);
            right_rotate(root);
        }
        // right left case
        if (balance < -1 && value < root->right->value) {
            right_rotate(root->right);
            left_rotate(root);
        }
    }
    void inorder_(Node* n) {
        if (n != nullptr) {
            inorder_(n->left);
            cout << n->value << endl;
            inorder_(n->right);
        }
    }
    void preorder_(Node* n) {
        if (n != nullptr) {
            cout << n->value << endl;
            preorder_(n->left);
            preorder_(n->right);
        }
    }

 public:
    AVL_Tree() {
        head = nullptr;
    }
    void add(int value) {
        add_(head, value);
    }
    void preorder() {
        preorder_(head);
    }
    void inorder() {
        inorder_(head);
    }
};

int main() {
    AVL_Tree tree;
    tree.add(1);
    tree.add(2);
    tree.add(3);
    tree.add(4);
    tree.add(5);
    tree.inorder();
    cout << endl;
    tree.preorder();
    cout << endl;
    return 0;
}
