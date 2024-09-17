// 实现 linear select 算法
// 从数组中找到第 k 小的元素

#ifndef LINEAR_SELECT_H
#define LINEAR_SELECT_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

// 输入：规模为 n 的无序序列 A，秩 k >= 0, 数据分组规模 m (默认为 5)
// 输出：A 所对应的有序序列中秩为 k 的元素
template<typename T>
int linearSelect(vector <T> &arr, int k, int m = 5) {
    int n = arr.size();
    // 递归终止条件：序列规模小于 m，直接排序返回结果
    if (n <= m) {
        sort(arr.begin(), arr.end());
        return arr[k];
    }
    // 每组 m 个元素，最后一组可能不足 m 个
    int group = (n + m - 1) / m;
    // 存储每组的中位数
    vector <T> medians(group);
    for (int i = 0; i < group; i++) {
        int start = i * m;
        int end = min(start + m - 1, n - 1);
        sort(arr.begin() + start, arr.begin() + end + 1);
        medians[i] = arr[(start + end) / 2];
    }
    // 递归地求出中位数的中位数
    T pivot = linearSelect(medians, group / 2, m);
    // 对 arr 进行 partition 操作
    // 将 arr 分成三部分, L: < pivot, E: == pivot, G: > pivot
    vector <T> L, E, G;
    for (int i = 0; i < n; i++) {
        if (arr[i] < pivot)
            L.push_back(arr[i]);
        else if (arr[i] == pivot)
            E.push_back(arr[i]);
        else
            G.push_back(arr[i]);
    }
    // 根据 L 和 E 的大小，判断 pivot 的秩
    if (k < L.size())
        return linearSelect(L, k, m);
    else if (k < L.size() + E.size())
        return pivot;
    else
        return linearSelect(G, k - L.size() - E.size(), m);
}

#endif