// 实现 quick select 算法
// 从数组中找到第 k 小的元素

#ifndef QUICK_SELECT_H
#define QUICK_SELECT_H

#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

// 对 arr[l...r] 部分进行 partition 操作
// 返回 p，使得 arr[l...p-1] < arr[p]; arr[p+1...r] > arr[p]
template <typename T>
int partition(vector<T> &arr, int l, int r)
{
    swap(arr[l], arr[rand() % (r - l + 1) + l]);
    T v = arr[l];
    // arr[l+1...j] < v; arr[j+1...i) > v
    int j = l;
    for (int i = l + 1; i <= r; i++)
    {
        if (arr[i] < v)
        {
            swap(arr[j + 1], arr[i]);
            j++;
        }
    }
    swap(arr[l], arr[j]);
    return j;
}

// 对 arr[l...r] 部分进行快速选择，返回第 k 小的元素
template <typename T>
T quickSelect(vector<T> &arr, int l, int r, int k)
{
    if (l == r)
        return arr[l];
    int p = partition(arr, l, r);
    if (k == p)
        return arr[p];
    else if (k < p)
        return quickSelect(arr, l, p - 1, k);
    else
        return quickSelect(arr, p + 1, r, k);
}

// 寻找第 k 小的元素
template <typename T>
T quickSelect(vector<T> &arr, int k)
{
    srand(time(NULL));
    return quickSelect(arr, 0, arr.size() - 1, k);
}

#endif
