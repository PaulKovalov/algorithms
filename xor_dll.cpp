#include <iostream>
using namespace std;

class xored_dll {
	struct Node {
		Node *nxp;
		int data;
		Node(int n):data(n);
	};
public:

	Node* head;
	xored_dll() {}

	void insert(int n) {
		if (head == 0) {
			head = new Node(n);
			head->nxp = 0; // xor of null and null
		} else {
			Node *node = new Node(n);
			head->nxp ^= node;
			node->nxp = node;
			head = node;
		}
	}

	void ltr_traversal() {
		Node* prev = 0, *current = head, *prev_copy = prev;
		while(current != nullptr) {
			cout << current->data << " ";
			prev = current;
			current = current->nxp ^ prev_copy;
			prev_copy = prev;
		}
		cout << endl;
	}

	void rtl_traversal() {

	}
};

int main() {
	xored_dll list;
	list.insert(1);
	list.insert(2);
	list.insert(3);
	list.insert(4);
	list.insert(5);
	list.ltr_traversal();
	return 0;
}