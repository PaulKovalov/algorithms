#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::pair;
int main() {

    int amount_of_items, volume, x, y;
    cout << "Enter amount of items: " << endl;
    cin >> amount_of_items;
    cout << "Enter knapsack volume: " << endl;
    cin >> volume;
    vector<vector<int> > table(amount_of_items+1, vector<int>(volume+1, 0));
    
    vector<pair<int,int> > items;

    for (int i = 0; i < amount_of_items; ++i) {
        cin >> x; // weight
        cin >> y; // cost
        items.push_back(std::make_pair(x,y));
    }


    for (int i = 1; i <= amount_of_items; ++i) {
        for (int j = 0; j <= volume; ++j) {
            if (items[i].first > j) {
                table[i][j] = table[i-1][j];
            } else {
                table[i][j] = max(table[i - 1][j], table[i-1][j - items[i].first] + items[i].second);        
            }
            
        }
    }
    cout << table[amount_of_items][volume] << endl;
    
    return 0;
}