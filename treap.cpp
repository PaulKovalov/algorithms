#include <iostream>
#include <queue>
#include <cstdio>

using namespace std;
template <class T>
class Treap {
    struct Node {
        T key;
        int priority;
        Node *left, *right;
        Node(T k, int p) : key(k), priority(p), left(nullptr), right(nullptr) {}
    };
    // Node *n will be a new treap
    void merge(Node *&n, Node *left, Node *right) {
        if (left == nullptr || right == nullptr) {
            n = left ? left : right;
        }
        else {
            if (left->priority > right->priority) {
                n = left;
                merge(n->right, left->right, right);
            }
            else {
                n = right;
                merge(n->left, left, right->left);
            }
        }
    }
    void split(Node *root, T key, Node *&l, Node *&r) {
        if (!root)
            l = r = NULL;
        else if (root->key > key) {
            r = root;
            split(root->left, key, l, root->left);
        }
        else
        {
            l = root;
            split(root->right, key, root->right, r);
        }
    }
    Node *insert(Node *root, Node *node)
    {
        if (!root)
            return node;
        if (node->priority > root->priority)
        {   
            split(root, node->key, node->left, node->right);
            return node;
        }
        else
        {
            if (node->key < root->key)
            {
                root->left = insert(root->left, node);
            }
            else
            {
                root->right = insert(root->right, node);
            }
            return root;
        }
    }
    void erase(Node *&n, T value)
    {
        if (n == nullptr)
        {
            return;
        }
        if (n->key == value)
        {
            merge(n, n->left, n->right);
        }
        else
        {

            if (n->key > value)
            {
                erase(n->left, value);
            }
            else
            {
                erase(n->right, value);
            }
        }
    }
    Node *head;

  public:
    Treap()
    {
        srand(time(NULL));
        head = 0;
    }
    void add(T value)
    {
        Node *n = new Node(value, rand() % 1000);
        head = insert(head, n);
    }
    void erase(T value)
    {
        erase(head, value);
    }
    void print()
    {
        queue<Node *> qn;
        qn.push(head);
        while (!qn.empty())
        {
            cout << "(" << qn.front()->key << ", " << qn.front()->priority << ") ";
            if (qn.front()->left != nullptr)
            {
                qn.push(qn.front()->left);
            }
            if (qn.front()->right != nullptr)
            {
                qn.push(qn.front()->right);
            }
            qn.pop();
        }
        cout << endl;
    }
};

int main()
{

    Treap<int> treap;

    cout << "Adding 5 numbers" << endl;

    treap.add(5);
    treap.add(8);
    treap.add(10);
    treap.add(11);
    treap.add(15);

    treap.print();
    cout << endl;
    cout << "Erasing 10" << endl;
    treap.erase(10);
    treap.print();
    cout << endl;
    return 0;
}