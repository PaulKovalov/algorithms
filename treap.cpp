#include <iostream>
#include <queue>
#include <cstdio>

using namespace std;
template <class T>
class Treap
{
    struct Node
    {
        T key;
        int priority;
        Node *left, *right;
        Node(T k, int p) : key(k), priority(p), left(nullptr), right(nullptr) {}
    };
    // Node *n will be a new treap
    void merge(Node *n, Node *left, Node *right)
    {
        if (left == nullptr || right == nullptr)
        {
            n = left ? left : right;
        }
        if (right->priority > left->priority)
        {
            n = right;
            merge(n->left, left, right->left);
        }
        else
        {
            n = left;
            merge(n->right, left->right, right);
        }
    }
    void split(Node *root, T key, Node *&l, Node *&r)
    {
        if (!root)
            l = r = NULL;
        else if (root->key > key)
        {
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
    void print() {
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
    treap.add(5);
    treap.add(10);
    treap.add(11);
    treap.print();
    return 0;
}