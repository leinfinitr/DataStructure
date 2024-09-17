#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>

using namespace std;

template<typename T>
class node {
public:
    T key;
    int height;
    node *left;
    node *right;

    explicit node(T k) {
        height = 1;
        key = k;
        left = nullptr;
        right = nullptr;
    }
};

template<typename T>
class AVL {
public:
    node<T> *root = nullptr;
    int num = 0;

    void insert(T x) {
        insertUtil(root, x);
    }

    void remove(T x) {
        removeUtil(root, x);
    }

    node<T> *search(T x) {
        return searchUtil(root, x);
    }

    void inorder() {
        inorderUtil(root);
        cout << endl;
    }

    void makeEmpty(node<T> *head) {
        if (head == nullptr) return;
        makeEmpty(head->left);
        makeEmpty(head->right);
        delete head;
    }

private:
    int height(node<T> *head) {
        if (head == nullptr) return 0;
        return head->height;
    }

    node<T> *rightRotation(node<T> *head) {
        node<T> *newHead = head->left;
        head->left = newHead->right;
        newHead->right = head;
        head->height = 1 + max(height(head->left), height(head->right));
        newHead->height = 1 + max(height(newHead->left), height(newHead->right));
        return newHead;
    }

    node<T> *leftRotation(node<T> *head) {
        node<T> *newHead = head->right;
        head->right = newHead->left;
        newHead->left = head;
        head->height = 1 + max(height(head->left), height(head->right));
        newHead->height = 1 + max(height(newHead->left), height(newHead->right));
        return newHead;
    }

    node<T> *leftRightRotation(node<T> *head) {
        head->left = leftRotation(head->left);
        node<T> *newHead = rightRotation(head);
        return newHead;
    }

    node<T> *rightLeftRotation(node<T> *head) {
        head->right = rightRotation(head->right);
        node<T> *newHead = leftRotation(head);
        return newHead;
    }

    bool adjust(node<T> *&t, int sub_tree) {
        if (sub_tree) {
            // 在右子树上删除使右子树变矮
            if (height(t->left) - height(t->right) == 1) return true;
            if (height(t->right) == height(t->left)) {
                --t->height;
                return false;
            }
            if (height(t->left->right) > height(t->left->left)) {
                t = leftRightRotation(t);
                return false;
            }
            t = rightRotation(t);
            if (height(t->right) == height(t->left)) return false;
            else return true;
        } else {
            // 在左子树上删除使左子树变矮
            if (height(t->right) - height(t->left) == 1) return true;
            if (height(t->right) == height(t->left)) {
                --t->height;
                return false;
            }
            if (height(t->right->left) > height(t->right->right)) {
                t = rightLeftRotation(t);
                return false;
            }
            t = leftRotation(t);
            if (height(t->right) == height(t->left)) return false;
            else return true;
        }
    }

    // Traverse and print AVL tree
    void inorderUtil(node<T> *head) {
        if (head == NULL) return;
        inorderUtil(head->left);
        cout << head->key << " ";
        inorderUtil(head->right);
    }

    void insertUtil(node<T> *&head, T x) {
        if (head == nullptr)
            head = new node<T>(x);
        else if (x == head->key) {
            return;
        } else if (x < head->key) {
            insertUtil(head->left, x);
            if (height(head->left) - height(head->right) == 2) {
                if (x < head->left->key) head = rightRotation(head);
                else head = leftRightRotation(head);
            }
        } else if (head->key < x) {
            insertUtil(head->right, x);
            if (height(head->right) - height(head->left) == 2) {
                if (head->right->key < x) head = leftRotation(head);
                else head = rightLeftRotation(head);
            }
        }

        head->height = max(height(head->left), height(head->right)) + 1;
    }

    bool removeUtil(node<T> *&head, T x) {
        if (head == nullptr) return true;
        if (x == head->key) {
            if (head->left == nullptr || head->right == nullptr) {
                // 被删节点是叶节点或只有一个儿子
                node<T> *old_node = head;
                head = (head->left != nullptr) ? head->left : head->right;
                delete old_node;
                return false;
            } else {
                // 被删节点有两个儿子
                node<T> *tmp = head->right;
                while (tmp->left != nullptr) tmp = tmp->left;
                head->key = tmp->key;
                if (removeUtil(head->right, x)) return true;
                return adjust(head, 1);
            }
        }

        if (x < head->key) {
            // 在左子树上删除
            if (removeUtil(x, head->left)) return true;
            // 删除后左子树没有变矮
            return adjust(head, 0);
        } else {
            // 在右子树上删除
            if (removeUtil(x, head->right)) return true;
            // 删除后右子树没有变矮
            return adjust(head, 1);
        }
    }

    node<T> *searchUtil(node<T> *head, T x) {
        if (head == nullptr) return nullptr;
        T k = head->key;
        if (k == x) return head;
        if (k > x) return searchUtil(head->left, x);
        if (k < x) return searchUtil(head->right, x);
        return nullptr;
    }
};

template<typename T>
class BST {
public:
    node<T> *root = nullptr;
    int num = 0;

    void insert(T x) {
        insertUtil(root, x);
    }

    void remove(T x) {
        removeUtil(root, x);
    }

    node<T> *search(T x) {
        return searchUtil(root, x);
    }

    void inorder() {
        inorderUtil(root);
        cout << endl;
    }

    void makeEmpty(node<T> *head) {
        if (head == nullptr) return;
        makeEmpty(head - left);
        makeEmpty(head->right);
        delete head;
    }

private:
    int height(node<T> *head) {
        if (head == NULL) return 0;
        return head->height;
    }

    // Traverse and print BST
    void inorderUtil(node<T> *head) {
        if (head == NULL) return;
        inorderUtil(head->left);
        cout << head->key << " ";
        inorderUtil(head->right);
    }

    void insertUtil(node<T> *&head, T x) {
        if (head == nullptr)
            head = new node<T>(x);
        else if (x == head->key)
            return;
        else if (x < head->key)
            insertUtil(head->left, x);
        else if (head->key < x)
            insertUtil(head->right, x);

        head->height = max(height(head->left), height(head->right)) + 1;
    }

    void removeUtil(node<T> *&head, T x) {
        if (head == nullptr) return;

        if (x < head->key)
            removeUtil(x, head->left);
        else if (x > head->key)
            removeUtil(x, head->right);
        else if (head->left != nullptr && head->right != nullptr) {
            node<T> *tmp = head->right;
            while (tmp->left != nullptr) tmp = tmp->left;
            head->key = tmp->key;
            remove(head->right, head->key);
        } else {
            node<T> *old_node = head;
            head = (head->left != nullptr) ? head->left : head->right;
            delete old_node;
        }
    }

    node<T> *searchUtil(node<T> *head, T x) {
        if (head == nullptr) return nullptr;
        T k = head->key;
        if (k == x) return head;
        if (k > x) return searchUtil(head->left, x);
        if (k < x) return searchUtil(head->right, x);
        return nullptr;
    }
};

int main() {
    // 在此处设置插入元素个数
    int numNum[] = {50, 100, 500, 1000, 2000};

    LARGE_INTEGER start_time, end_time, frequency;
    double elapsed_time_avl, elapsed_time_bst;
    srand(time(nullptr));
    for (auto i: numNum) {
        AVL<int> avl;
        BST<int> bst;
        vector<int> num_list;
        vector<int> search_list;
        for (int j = 1; j <= i; j++) {
            int randNum = rand();
            num_list.push_back(randNum);
            if (j % (i / 10) == 0) search_list.push_back(randNum);
        }

        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start_time);
        for (auto m: num_list)
            avl.insert(m);
        QueryPerformanceCounter(&end_time);
        elapsed_time_avl = static_cast<double>(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;

        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start_time);
        for (auto m: num_list)
            bst.insert(m);
        QueryPerformanceCounter(&end_time);
        elapsed_time_bst = static_cast<double>(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;

        cout << "含有 " << i << " 个随机元素的AVL树和BST构建完毕！" << endl;
        // cout << "中序遍历： ";
        // avl.inorder();
        cout << "AVL树的高度为: " << avl.root->height << endl;
        cout << "BST的高度为:  " << bst.root->height << endl;
        cout << "AVL树构建用时: " << elapsed_time_avl * 1000 << "ms" << endl;
        cout << "BST构建用时:  " << elapsed_time_bst * 1000 << "ms" << endl;
        cout << "AVL树任意十个元素的检索时间分别为：" << endl;
        for (auto k: search_list) {
            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&start_time);
            node<int> *res = avl.search(k);
            QueryPerformanceCounter(&end_time);
            elapsed_time_avl = static_cast<double>(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
            cout << elapsed_time_avl * 1000 << "ms  ";
        }
        cout << endl;
        cout << "BST任意十个元素的检索时间分别为：" << endl;
        for (auto k: search_list) {
            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&start_time);
            node<int> *res = bst.search(k);
            QueryPerformanceCounter(&end_time);
            elapsed_time_bst = static_cast<double>(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
            cout << elapsed_time_bst * 1000 << "ms  ";
        }
        cout << "\n\n";
    }
}
