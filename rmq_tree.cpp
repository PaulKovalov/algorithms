  #include <iostream>
  #include <vector>
  #include <iterator>

  using namespace std;
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
            euler_tour.push_back(n->priority);
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
        void preorder_(Node *n) {
            if (n != nullptr) {
            cout << "Root: "<< n->key << ", "<< n->priority << endl;
            cout << "Left: ";
            if (n->left == nullptr) { 
                cout << "nullptr" << endl;
            } else {
                cout << n->left->key << ", " << n->left->priority << endl; 
            }
            cout << "Right: ";
            if (n->right == nullptr) { 
                cout << "nullptr" << endl;
            } else {
                cout << n->right->key << ", " << n->right->priority << endl; 
            }
            cout << endl;
            preorder_(n->left);
            preorder_(n->right);
            }
        }
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
        void print(){
            preorder_(head);
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
    int main() {
        int n;
        cin >> n;
        vector<int> v(n);
        for (int i = 0; i < n; ++i) {
            cin >> v[i];
        }
        Tree tree(n);
        tree.build_tree(v);
        cout << "Tree: " << endl;
        tree.print();
        cout << endl;
        
        tree.dfs();
        vector<int> euler = tree.get_euler();
        vector<int> heights = tree.get_heights();
        vector<int> first = tree.get_first();
        cout <<"Euler tour: "<< endl;
        copy(euler.begin(), euler.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "Heights: " << endl; 
        copy(heights.begin(), heights.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "First: " << endl; 
        copy(first.begin(), first.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        return 0;
    }