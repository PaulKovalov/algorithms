#include <iostream>
#include <vector>

using namespace std;
template <class T>
class BinHeap {
    vector<T> v;
    int heap_size;

  public:
    BinHeap() {
        heap_size = 0;
    }
    void add(T value) {

        ++heap_size;
        v.push_back(value);
        int i = heap_size - 1;
        int parent = (i - 1) / 2;

        while (i > 0 && v[parent] < v[i]) {
            // swap them
            swap(v[parent], v[i]);
            i = parent;
            parent = (i - 1) / 2;
        }
    }
    void heapify(int i) {
        int left_child;
        int right_child;
        int largest_child;

        while(true) {
            left_child = 2*i + 1;
            right_child = 2*i + 2;
            largest_child = i;

            if (left_child < heap_size && v[left_child] > v[largest_child]) {
                largest_child = left_child;
            }
            if (right_child < heap_size && v[right_child] > v[largest_child]) {
                largest_child = right_child;
            }
            if (largest_child == i) {
                break;
            }
            swap(v[i], v[largest_child]);
            i = largest_child;
        }
    }
    void print_heap() {
        for (int i = 0; i < heap_size; ++i) {
            cout << v[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    BinHeap<int> binHeap;
    binHeap.add(1);
    binHeap.add(2);
    binHeap.add(3);
    binHeap.add(4);
    binHeap.add(5);

    binHeap.print_heap();
    return 0;
}