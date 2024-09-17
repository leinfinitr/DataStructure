#include <algorithm>
#include <cstdint>
#include <stack>
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>

class RandGenerator {
private:
    uint64_t seed = 0;
    constexpr static int32_t
    BASE = 1103515245;
    constexpr static int32_t
    OFFSET = 12345;
    constexpr static int32_t
    MOD = 1e9 + 7;

public:
    uint32_t treap_rand() {
        seed = (BASE * seed + OFFSET) % MOD;
        return seed;
    }

    void reset() {
        seed = 0;
    }
};

template<typename T>
class Treap;

template<typename T>
class TreapNode {

private:
    T val;
    TreapNode<T> *left;
    TreapNode<T> *right;
    int32_t count; /* number of repeated elements contained in current node */
    int32_t size;  /* number of nodes in current subtree */
    uint32_t weight;

    friend class Treap<T>;

public:
    TreapNode() : val(T()), left(nullptr), right(nullptr), count(0), size(0), weight(0) {}

    TreapNode(T val, TreapNode *left, TreapNode *right,
              int32_t count, int32_t size, uint32_t weight, int height) : val(val), left(left), right(right),
                                                                          count(count), size(size), weight(weight) {}
};

template<typename T>
class Treap {

private:
    RandGenerator rand;
    int size = 0;

    TreapNode<T> *treap_root = nullptr;

public:
    void insert(T val) {
        // 分裂操作
        TreapNode<T> *left_root = nullptr, *right_root = nullptr;
        split(treap_root, left_root, right_root, val);
        // 若左子树中存在该节点，则 count++
        // 若不存在在则创建新节点
        TreapNode<T> *res = nullptr, *new_node = nullptr;
        if (search(left_root, val, res)) {
            res->count++;
            return;
        } else {
            new_node = new TreapNode<T>;
            new_node->val = val;
            new_node->count = 1;
            new_node->size = 1;
            new_node->weight = rand.treap_rand();
            new_node->left = nullptr;
            new_node->right = nullptr;
        }
        // 将左子树和新节点进行合并
        left_root = merge(left_root, new_node);
        // 将新的左子树和右节点进行合并
        treap_root = merge(left_root, right_root);
        size++;
    }

    void remove(T val) {
        // 将树划分为⼩于等于val的左⼦树和大于val的sub右⼦树
        TreapNode<T> *left_root = nullptr, *right_root = nullptr;
        split(treap_root, left_root, right_root, val);
        // 将左子树划分为⼩于val的sub左⼦树和等于val的sub右⼦树
        TreapNode<T> *sub_left_root = nullptr, *sub_right_root = nullptr;
        split(left_root, sub_left_root, sub_right_root, val - 1);
        // 如果sub右⼦树存在，则将其count--
        // 如果count为0，则删除该节点
        if (sub_right_root != nullptr) {
            if (sub_right_root->count > 1)
                sub_right_root->count--;
            else {
                delete sub_right_root;
                sub_right_root = nullptr;
                size--;
            }
        };
        // 将sub左⼦树和sub右⼦树合并
        left_root = merge(sub_left_root, sub_right_root);
        // 将左⼦树和右⼦树合并
        treap_root = merge(left_root, right_root);
    }

    /*
    @brief  查询 val 的前驱
     */
    T pre_element(T val) {
        // 查询 val 的排名
        int32_t rk = rank(val);
        // 若val不存在，则返回中遍历中比val小的最大值
        if (rk == -1) {
            std::vector <T> vec;
            in_traverse(treap_root, vec);
            for (int i = vec.size() - 1; i >= 0; i--) {
                if (vec[i] < val)
                    return vec[i];
            }
            return -1;
        }
        // 查询排名为 rk - 1 的元素
        return kth_element(rk - 1);
    }

    /*
    @brief  查询 val 的后继
    */
    T suc_element(T val) {
        // 查询 val 的排名
        int32_t rk = rank(val);
        // 若val不存在，则返回中遍历中比val大的最小值
        if (rk == -1) {
            std::vector <T> vec;
            in_traverse(treap_root, vec);
            for (int i = 0; i < vec.size(); i++) {
                if (vec[i] > val)
                    return vec[i];
            }
            return -1;
        }
        // 查询排名为 rk + 1 的元素
        return kth_element(rk + 1);
    }

    /*
    @brief  查询 val 的排名
    @return 返回 val 的排名
    */
    int32_t rank(T val) {
        // 得到整棵树中序遍历的结果
        std::vector <T> vec;
        in_traverse(treap_root, vec);
        // 在vec中查找val的排名
        for (int i = 0; i < vec.size(); i++) {
            if (vec[i] == val)
                return i + 1;
        }
        return -1;
    }

    /*
    @brief 查询排名为 rk 的元素
    */
    int32_t kth_element(int32_t rk) {
        // 得到整棵树前序遍历的结果
        std::vector <T> vec;
        in_traverse(treap_root, vec);
        // 返回排名为 rk 的元素
        if (rk > vec.size() || rk < 1)
            return -1;
        return vec[rk - 1];
    }

    /*
    @brief 删除整棵树并释放动态内存
    */
    void clear() {
        if (treap_root == nullptr)
            return;
        std::stack < TreapNode<T> * > stack;
        stack.push(treap_root);
        while (!stack.empty()) {
            TreapNode<T> *cur = stack.top();
            stack.pop();
            if (cur->left)
                stack.push(cur->left);
            if (cur->right)
                stack.push(cur->right);
            delete cur;
        }
        treap_root = nullptr;
        rand.reset();
    }

    /*
    @brief  前序遍历整棵树
    */
    std::string pre_traverse() {
        if (treap_root == nullptr)
            return "empty";
        std::string res;
        // 将前序遍历结果存储在vector中
        std::vector <T> vec;
        pre_traverse(treap_root, vec);
        // 将vector中的元素转换为字符串
        for (auto &i: vec) {
            res += std::to_string(i);
            res += " ";
        }
        return res;
    }

    /*
    @brief 前序遍历以root为根的树, 并将结果存储在vec中
    */
    void pre_traverse(TreapNode<T> *root, std::vector <T> &vec) {
        if (root == nullptr)
            return;
        vec.push_back(root->val);
        pre_traverse(root->left, vec);
        pre_traverse(root->right, vec);
    }

    /*
    @brief 中序遍历以root为根的树, 并将结果存储在vec中
    */
    void in_traverse(TreapNode<T> *root, std::vector <T> &vec) {
        if (root == nullptr)
            return;
        in_traverse(root->left, vec);
        vec.push_back(root->val);
        in_traverse(root->right, vec);
    }

    /*
     * @brief 将树分裂为⼩于等于val的左⼦树和大于val的右⼦树
     * @param root 树根
     * @param left_root 分裂后的左⼦树根
     * @param right_root 分裂后的右⼦树根
     * @param value 分裂值
     */
    void split(TreapNode<T> *root, TreapNode<T> *&left_root, TreapNode<T> *&right_root, T value) {
        if (root == nullptr)
            return;
        left_root = root;
        right_root = root;
        if (value >= root->val) {
            while (right_root->right && right_root->right->val <= value)
                right_root = right_root->right;
            if (!right_root->right) {
                right_root = nullptr;
                return;
            }
            TreapNode<T> *tmp = right_root->right;
            TreapNode<T> *sub_left = nullptr, *sub_right = nullptr;
            split(tmp->left, sub_left, sub_right, value);
            right_root->right = sub_left;
            tmp->left = sub_right;
            right_root = tmp;
        } else {
            while (left_root->left && left_root->left->val > value)
                left_root = left_root->left;
            if (!left_root->left) {
                left_root = nullptr;
                return;
            }
            TreapNode<T> *tmp = left_root->left;
            TreapNode<T> *sub_left = nullptr, *sub_right = nullptr;
            split(tmp->right, sub_left, sub_right, value);
            left_root->left = sub_right;
            tmp->right = sub_left;
            left_root = tmp;
        }
    }

    /*
        @brief 查找该值是否存在
        @param root 树根
        @param val 查找值
        @param res 查找结果
        @return 是否存在
    */
    bool search(TreapNode<T> *root, T val, TreapNode<T> *&res) {
        if (root == nullptr)
            return false;
        if (root->val == val) {
            res = root;
            return true;
        }
        if (val < root->val)
            return search(root->left, val, res);
        if (val > root->val)
            return search(root->right, val, res);
        return false;
    }

    /*
    @brief 合并两个树
    @param left_node 左树根
    @param right_node 右树根
    @return 合并后的树根
    */
    TreapNode<T> *merge(TreapNode<T> *left_node, TreapNode<T> *right_node) {
        if (left_node == nullptr)
            return right_node;
        if (right_node == nullptr)
            return left_node;
        // 保持最大堆性质
        TreapNode<T> *res = nullptr;
        if (left_node->weight >= right_node->weight) {
            left_node->right = merge(left_node->right, right_node);
            res = left_node;
        } else {
            right_node->left = merge(left_node, right_node->left);
            res = right_node;
        }
        return res;
    }

    // 得到整棵树的高度
    int get_height() {
        return get_height(treap_root);
    }

    // 得到以root为根的树的高度
    int get_height(TreapNode<T> *root) {
        if (root == nullptr)
            return 0;
        return std::max(get_height(root->left), get_height(root->right)) + 1;
    }

    // 得到整棵树的节点数
    int32_t get_size() {
        return size;
    }
};
