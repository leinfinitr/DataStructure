//
// Created by LEinfinitr on 2023/2/27.
//

#include "skiplist.h"

// 从跳表中查找数值，返回值为是否存在该数值
// search_value 为要查找的数值， search_step 为本次查找步数
bool Skiplist::search(int search_value, int &search_step)
{
    SkiplistNode* res;  // 存储本次查找到的节点的位置
    search_step = 1;    // 初始化 search_step 为 1
    // 从跳表的头节点开始查找
    return search(head, res, search_value, search_step);
}

// 从某一个结点开始，向右向下查找数值
// 若查找成功，则返回 true，并且 pos 指向被查找的元素
// 若查找失败，则返回 false，并且 pos 指向最后一个被查找的元素
// 使用该函数需保证 search_head 不为空指针
bool Skiplist::search(SkiplistNode *search_head, SkiplistNode *&pos, int search_value, int &search_step)
{
    // 若查找起始节点为空，则直接返回 false
    if(search_head == nullptr) return false;
    // 若起始节点的后方节点不为空，且要查找的值（下称目标值）大于后方节点的值，则一直向该层后方查找
    while (search_head->next != nullptr && search_head->next->value <= search_value){
        search_head = search_head->next;
        search_step++;  // 每向后走一步，查找步数+1
    }

    // 若当前节点的值与目标值相同，则返回 true
    if(search_head->value == search_value){
        pos = search_head;
        return true;
    }
    // 若当前节点下方节点为空，则代表已经检索到最后一个节点，查找失败
    if(search_head->below == nullptr){
        pos = search_head;
        return false;
    }
    // 否则进入下一层查找，同时查找步数+1
    return search(search_head->below, pos ,search_value, ++search_step);
}

// 插入算法
void Skiplist::insert(int insert_value)
{
    SkiplistNode* pos;  // 保存查找算法的最终查找节点
    int search_step = 1;    // 查找步数，无实际意义，仅为满足参数要求
    // 若查找成功，则直接返回
    if(search(head, pos, insert_value, search_step))
        return;
    else{
        // 首先在最底层插入节点
        // 经过查找算法，pos 指向被插入位置的前一个结点
        SkiplistNode* tmp = pos->next;  // 保存插入位置后一个节点的地址
        SkiplistNode* new_node = new SkiplistNode(insert_value);    // 创建节点
        // 节点互相链接
        pos->next = new_node;
        new_node->next = tmp;
        new_node->pre = pos;
        if(tmp != nullptr) tmp->pre = new_node;
        // 让 tmp 指向当前节点
        tmp = new_node;
        // 向上成塔
        while (rand() % 100 <= 13){    // 按照 1/2 概率生长
            // 找到当前层上一个通向上一层的节点
            while (tmp->pre != nullptr && tmp->above == nullptr)
                tmp = tmp->pre;
            // 如果达到整个跳表的头节点
            if(tmp->pre == nullptr && tmp->above == nullptr){
                // 新建一层，并让 head 指向新的头节点
                tmp->above = new SkiplistNode();
                head = tmp->above;
                // 让新的头节点的 below 指向旧的头节点
                head->below = tmp;
                // 创建顶层的增长节点
                SkiplistNode* above_node = new SkiplistNode(insert_value);
                // 节点互相链接
                head->next = above_node;
                above_node->pre = head;
                above_node->below = new_node;
                new_node->above = above_node;
                // 层数+1
                level++;
                break;
            } else {    // 未达到整个跳表的头节点，即 tmp->above != nullptr
                // 创建增长节点
                SkiplistNode* above_node = new SkiplistNode(insert_value);
                // 增长节点与相邻节点的互相链接
                SkiplistNode* tmp2 = tmp->above->next;
                tmp->above->next = above_node;
                above_node->pre = tmp->above;
                above_node->next = tmp2;
                above_node->below = new_node;
                new_node->above = above_node;
                if(tmp2 != nullptr) tmp2->pre = above_node;

                // 更改 new_node 和 tmp 指向的位置，进行下一次循环
                tmp = new_node = above_node;
            }
        }
    }
}

// 打印跳表
void Skiplist::print() const
{
    SkiplistNode* p = head;
    while (p != nullptr){
        SkiplistNode* p2 = p->next;
        while (p2 != nullptr) {
            std::cout << p2->value << " ";
            p2 = p2->next;
        }
        std::cout << "\n";
        p = p->below;
    }
}