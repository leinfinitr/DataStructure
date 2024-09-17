#ifndef HW2_SKIPLIST_H
#define HW2_SKIPLIST_H

#include <random>
#include <iostream>
#include <ctime>
#include <cmath>

// 跳表中的所有值均为正整数
class SkiplistNode{
public:
    int value;
    SkiplistNode* pre;
    SkiplistNode* next;
    SkiplistNode* below;
    SkiplistNode* above;

    explicit SkiplistNode(int v = 0, SkiplistNode* p = nullptr, SkiplistNode* n = nullptr,
                 SkiplistNode* b = nullptr, SkiplistNode* a = nullptr){
        value = v;
        pre = p;
        next = n;
        below = b;
        above = a;
    }
};

class Skiplist{
public:
    int level;
    SkiplistNode* head;

    Skiplist(){
        level = 1;
        head = new SkiplistNode();
    }

    bool search(int search_value, int &search_step);
    bool search(SkiplistNode* search_head, SkiplistNode* &pos, int search_value, int &search_step);
    void insert(int insert_value);
    void print() const;
};

#endif //HW2_SKIPLIST_H
