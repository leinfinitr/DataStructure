#include <iostream>
#include <string>

using namespace std;

int m = 20000;    // 哈希数组大小
int n = 10000;    // 集合中插入的元素个数

hash <string> hash1;

int main() {
    for (; m <= 50000; m += 10000) {
        for (int k = 1; k <= 8; k++) {
            // 构建大小为 m 的哈希数组
            int *hash_table = new int[m]();

            // 增加元素
            for (int i = 0; i < n; ++i) {
                string si = to_string(i);
                for (int j = 0; j < k; ++j) {
                    hash_table[hash1(si) % m] = 1;
                    si += "a";
                }
            }
            // 检验误报率
            int count = 0;  // 误报次数
            for (int i = n; i < m; ++i) {
                string si = to_string(i);
                int tmp = 0;
                for (int j = 0; j < k; ++j) {
                    if (hash_table[hash1(si) % m] == 1)
                        tmp++;
                    si += "b";
                }
                if (tmp == k) count++;
            }
            cout << double(count) / (m - n) << " ";
            delete[] hash_table;
        }
        cout << endl;
    }


    return 0;
}
