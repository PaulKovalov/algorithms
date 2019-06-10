#include <stack>
#include <utility>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

int main()
{
    srand(time(NULL));

    stack<pair<int, int>> s1, s2;
    vector<int> arr(10);
    for (int i = 0; i < 10; ++i) {
        arr[i] = rand() % 10;
        cout << arr[i] << " ";
    }
    cout << endl;   
    int k = 0;
    cin >> k;
    for (int i = 0; i < k; ++i) {
        int curr_min = s1.empty() ? arr[i] : min(arr[i], s1.top().second);
        s1.push(make_pair(arr[i],curr_min));
    }
    for (int i = k; i < 10; ++i) {
        if (s2.empty())
        {
            while (!s1.empty())
            {
                int element = s1.top().first;
                int curr_min = s2.empty() ? element : min(element, s2.top().second);
                s2.push(make_pair(element, curr_min));
                s1.pop();
            }
        }
        int mmin = 0;
        if (s1.empty() || s2.empty()) {
            mmin = s1.empty() ? s2.top().second : s1.top().second;
        } else {
            mmin = min(s1.top().second, s2.top().second);
        }
        cout << "Min is: " << mmin << endl;
        s2.pop();
        int curr_min = s1.empty() ? arr[i] : min(arr[i], s1.top().second);
        s1.push(make_pair(arr[i],curr_min));



    }
    

    return 0;
}