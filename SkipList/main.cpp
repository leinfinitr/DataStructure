#include <iostream>
#include "skiplist.h"

using namespace std;

int main() {
    Skiplist skiplist;
    for (int i = 1; i <= 100 ; ++i)
        skiplist.insert(i);

    cout << "skiplist level : " << skiplist.level << endl;
    // skiplist.print();
    int total_step = 0;
    srand(time(nullptr));
    // 随机搜索一万次，记录平均搜索长度
    for (int i = 0; i < 10000; ++i) {
        int step = 1;
        int search_num = rand() % 100 + 1;
        skiplist.search(search_num, step);
        total_step += step;
    }
    cout << "average search step : " << total_step / 10000.0;
    return 0;
}
