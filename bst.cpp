#include <iostream>
#include <queue>

using namespace std;

template<class T>
class BinarySearchTree {
    struct Node {
        T value;
        Node *left;
        Node *right;
    };
    Node *head;
    void add_(T value, Node *&node) {
        if (value > node->value)
        {
            if (node->right != 0)
            {
                add_(value, node->right);
            }
            else
            {
                node->right = new Node();
                node->right->value = value;
            }
        }
        else if (value < node->value)
        {
            if (node->left != 0)
            {
                add_(value, node->left);
            }
            else
            {
                node->left = new Node();
                node->left->value = value;
            }
        }
    }
    
    void inorder_(Node *node) {
        if (node->left != 0) {
            inorder_(node->left);
        }
        cout << node->value << endl;
        if (node->right != 0) {
            inorder_(node->right);
        }
    }
    void preorder_(Node* node) {
        cout << node->value << endl;
        if (node->left != 0) {
            preorder_(node->left);
        }
        if (node->right != 0) {
            preorder_(node->right);
        }
    }
  public:
    void add(T value) {
        if (head == 0)
        {
            head = new Node();
            head->value = value;
        }
        else
        {
            add_(value, head);
        }
    }
    void print_bfs() {
        queue<Node*> qn;
        qn.push(head);
        while (!qn.empty()) {
            cout << qn.front()->value << " ";
            if (qn.front()-> left != nullptr) {
                qn.push(qn.front()->left);
            }
            if (qn.front()-> right != nullptr) {
                qn.push(qn.front()->right);
            }
            qn.pop();
        }
    }
    BinarySearchTree() {
        head = 0;
    }
    void inorder() {
        inorder_(head);
    }
    void preorder() {
        preorder_(head);
    }
};
int main()
{
    BinarySearchTree<int> tree;
    
    tree.add(1);
    tree.add(19);
    tree.add(7);
    tree.add(5);
    tree.add(6);
    tree.add(42);
    tree.add(21);
	tree.add(13);
	tree.add(56);
	tree.add(78);
	tree.add(29);
	tree.add(3);
	tree.add(14);
    tree.print_bfs();
    cout << endl;
    tree.inorder();

    cout << endl;
    tree.preorder();
    
    return 0;
}
