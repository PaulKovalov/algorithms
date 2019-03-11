#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class sqrt_decompose
{
    vector<int> blocks;
    vector<int> a;
    vector<int> decompose()
    {
        double blocks_size = sqrt(a.size());
        vector<int> blocks(blocks_size, 0);
        for (int i = 0; i < a.size(); i++)
        {
            blocks[i / blocks_size] += a[i];
        }
        return blocks;
    }

  public:
    sqrt_decompose(vector<int> a)
    {
        this->a = a;
        this->blocks = decompose();
    }

    int query(int l, int r)
    {
        int left_block = l / blocks.size();
        int right_block = r / blocks.size();

        int query_result = 0;
        int operation_count = 0;
        if (left_block == right_block)
        {
            for (int i = l; i <= r; i++)
            {
                query_result += a[i];
                operation_count++;
            }
        }
        else
        {
            if (l == 0 || a.size() % l == 0)
            {
                query_result += blocks[left_block];
            }
            else
            {
                for (int i = l; i < (left_block + 1) * sqrt(a.size()); i++)
                {
                    query_result += a[i];
                    operation_count++;
                }
            }
            for (int i = left_block + 1; i < right_block; i++)
            {
                query_result += blocks[i];
                operation_count++;
            }
            for (int i = right_block * blocks.size(); i <= r; i++)
            {
                query_result += a[i];
                operation_count++;
            }
        }
        cout << "Total operations performed: " << operation_count << endl;
        return query_result;
    }
    
    void set_value(int index, int value) {
        int block = index / blocks.size();
        blocks[block] -= a[index];
        a[index] = value;
        blocks[block] += value;

    }
};

int main()
{
    vector<int> a;
    for (int i = 0; i < 100; i++)
    {
        a.push_back(i + 1);
    }
    sqrt_decompose sd(a);
    int l, r;
    while (true)
    {
        cout << "Enter query" << endl;
        cin >> l >> r;
        cout << sd.query(l, r) << endl;
    }

    return 0;
}