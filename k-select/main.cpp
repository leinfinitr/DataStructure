#include <iostream>
#include "linearSelect.h"
#include "quickSelect.h"

using namespace std;

// 精确测试函数的运行时间

int main()
{
    // 数据集规模
    int size[] = {100, 500, 1000, 2000, 3000};
    int sizeNum = sizeof(size) / sizeof(int);
    // 顺序测试集
    vector<vector<int>> ordered;
    // 乱序测试集
    vector<vector<int>> unordered;
    for (int i = 0; i < sizeNum; i++)
    {
        vector<int> o, u;
        for (int j = 0; j < size[i]; j++)
        {
            o.push_back(j);
            u.push_back(rand());
        }
        ordered.push_back(o);
        unordered.push_back(u);
    }

    // 测试 linearSelect 和 quickSelect 的正确性
    // for (int i = 0; i < size[0]; i++)
    //     cout << linearSelect(ordered[0], i) << " ";
    // cout << endl;
    // for (int i = 0; i < size[0]; i++)
    //     cout << quickSelect(ordered[0], i) << " ";
    // cout << endl;
    // for (int i = 0; i < size[0]; i++)
    //     cout << linearSelect(unordered[0], i) << " ";
    // cout << endl;
    // for (int i = 0; i < size[0]; i++)
    //     cout << quickSelect(unordered[0], i) << " ";
    // cout << endl;

    // 分别计算每种算法的运行时间，比较不同数据集下两种算法的性能
    // for (int i = 0; i < sizeNum; i++)
    // {
    //     cout << "size: " << size[i] << endl;

    //     cout << "ordered: " << endl;
    //     clock_t start = clock();
    //     for (int j = 0; j < size[i]; j++)
    //         linearSelect(ordered[i], j);
    //     clock_t end = clock();
    //     cout << "linearSelect: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    //     start = clock();
    //     for (int j = 0; j < size[i]; j++)
    //         quickSelect(ordered[i], j);
    //     end = clock();
    //     cout << "quickSelect: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    //     cout << "unordered: " << endl;
    //     start = clock();
    //     for (int j = 0; j < size[i]; j++)
    //         linearSelect(unordered[i], j);
    //     end = clock();
    //     cout << "linearSelect: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    //     start = clock();
    //     for (int j = 0; j < size[i]; j++)
    //         quickSelect(unordered[i], j);
    //     end = clock();
    //     cout << "quickSelect: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    // }

    // 测试 linearSelect 的分组规模分别为 5、10、15、20、25 时的性能
    int Q[] = {5, 10, 15, 20, 25};
    int QNum = sizeof(Q) / sizeof(int);
    for (int i = 0; i < sizeNum; i++)
    {
        cout << "size: " << size[i] << endl;
        cout << "ordered: " << endl;
        for (int j = 0; j < QNum; j++)
        {
            clock_t start = clock();
            for (int k = 0; k < size[i]; k++)
                linearSelect(ordered[i], k, Q[j]);
            clock_t end = clock();
            cout << "Q = " << Q[j] << ": " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
        }
        cout << "unordered: " << endl;
        for (int j = 0; j < QNum; j++)
        {
            clock_t start = clock();
            for (int k = 0; k < size[i]; k++)
                linearSelect(unordered[i], k, Q[j]);
            clock_t end = clock();
            cout << "Q = " << Q[j] << ": " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
        }
    }
}
