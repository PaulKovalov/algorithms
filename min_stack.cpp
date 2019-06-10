#include <stack>
#include <utility>
#include <algorithm>
#include <iostream>

using std::stack;
using std::min;
using std::pair;
using std::cin;
using std::cout;
using std::endl;

int main() {

    stack<pair<int, int> > s;

    int n, x;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> x;
        int min_el = s.empty() ? x : min(s.top().second, x);
        s.push(std::make_pair(x, min_el));
    }
    cout << s.top().second;
}