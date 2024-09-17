// 稀疏矩阵默认绝大多数元素为0
// main函数用于创建测试集并比较与朴素矩阵的性能 使用时需要在debug文件夹创建input和output文件夹

#include <iostream>
#include <ctime>
#include <random>
#include <sstream>
#include <windows.h>

#include "SparseMatrix.h"

const int num_case = 7; // 测试样例个数
const int ratio = 10;   // 系数矩阵中 0 的占比
int size_cases[] = {10, 50, 100, 200, 300, 400, 500};
std::string grade_cases[num_case];

int main() {
    // 创建测试集文件名称
    for (int i = 0; i < num_case; i++)
        grade_cases[i] = "test" + std::to_string(i + 1);

    // 初始化随机数种子
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 100); // 左闭右闭区间
    e.seed(time(0));

    // 创建测试数据
    std::ofstream out;
    for (int i = 0; i < num_case; ++i) {
        // 创建左矩阵
        out.open("input/" + grade_cases[i] + ".left");
        out << size_cases[i] << " " << size_cases[i] << "\n";
        for (int j = 0; j < size_cases[i]; ++j) {
            for (int k = 0; k < size_cases[i]; ++k) {
                int rand = u(e);
                if (rand > ratio)
                    out << j << " " << k << " " << rand << "\n";
            }
        }
        out.close();
        // 创建右矩阵
        out.open("input/" + grade_cases[i] + ".right");
        out << size_cases[i] << " " << size_cases[i] << "\n";
        for (int j = 0; j < size_cases[i]; ++j) {
            for (int k = 0; k < size_cases[i]; ++k) {
                int rand = u(e);
                if (rand > ratio)
                    out << j << " " << k << " " << rand << "\n";
            }
        }
        out.close();
    }

    // 记录使用稀疏矩阵乘法所需时间
    double time;
    LARGE_INTEGER nFreq;
    LARGE_INTEGER nBeginTime;
    LARGE_INTEGER nEndTime;
    for (const auto &test_case: grade_cases) {
        SparseMatrix left = SparseMatrix("input/" + test_case + ".left");
        SparseMatrix right = SparseMatrix("input/" + test_case + ".right");
        QueryPerformanceFrequency(&nFreq);
        QueryPerformanceCounter(&nBeginTime);
        SparseMatrix ans = left * right;
        QueryPerformanceCounter(&nEndTime);
        ans.creat_vector_col();
        time = (double) (nEndTime.QuadPart - nBeginTime.QuadPart) / (double) nFreq.QuadPart;
        std::cout << time * 1000 << " ";
    }
    std::cout << "\n";

    // 记录使用朴素矩阵乘法所需时间
    for (const auto &test_case: grade_cases) {
        // 朴素矩阵的创建与相乘
        // 左矩阵
        int row_left, col_left;
        std::ifstream in("input/" + test_case + ".left");
        in >> row_left >> col_left;
        int **left;
        left = new int *[row_left];
        for (int i = 0; i < row_left; ++i)
            left[i] = new int[col_left];
        for (int i = 0; i < row_left; ++i)
            for (int j = 0; j < col_left; ++j)
                left[i][j] = 0;
        int row, col, val;
        while (in >> row) {
            in >> col >> val;
            if (val == 0) continue;
            left[row][col] = val;
        }
        in.close();
        // 右矩阵
        int row_right, col_right;;
        in.open("input/" + test_case + ".right");
        in >> row_right >> col_right;
        int **right;
        right = new int *[row_right];
        for (int i = 0; i < row_right; ++i)
            right[i] = new int[col_right];
        for (int i = 0; i < row_right; ++i)
            for (int j = 0; j < col_right; ++j)
                right[i][j] = 0;
        while (in >> row) {
            in >> col >> val;
            if (val == 0) continue;
            right[row][col] = val;
        }
        in.close();
        // 相乘
        int **res;
        res = new int *[row_left];
        QueryPerformanceFrequency(&nFreq);
        QueryPerformanceCounter(&nBeginTime);
        for (int i = 0; i < row_left; ++i)
            res[i] = new int[col_right];
        for (int i = 0; i < row_left; ++i)
            for (int j = 0; j < col_right; ++j)
                res[i][j] = 0;
        for (int i = 0; i < row_left; ++i)
            for (int j = 0; j < col_left; ++j)
                for (int k = 0; k < col_left; ++k)
                    res[i][j] += left[i][k] * right[k][j];

        // 释放动态空间
        for (int i = 0; i < row_left; ++i)
            delete[] left[i];
        for (int i = 0; i < row_right; ++i)
            delete[] right[i];
        for (int i = 0; i < row_left; ++i)
            delete[] res[i];
        delete[] left;
        delete[] right;
        delete[] res;

        QueryPerformanceCounter(&nEndTime);
        time = (double) (nEndTime.QuadPart - nBeginTime.QuadPart) / (double) nFreq.QuadPart;
        std::cout << time * 1000 << " ";
    }

    return 0;
}

/*const int num_case = 3;
std::string grade_cases[num_case];

void compare_res(std::string test_case) {
    std::ifstream stu, ans;
    std::stringstream errstring;
    std::string s_stu, s_ans;
    int lineno = 0;

    std::string name = "output/" + test_case + ".yourans";
    stu.open(name, std::ios::in);
    if (!stu.is_open()) {
        throw std::runtime_error("Failed to open file: " + name);
    }

    name = "output/" + test_case + ".ans";
    ans.open(name, std::ios::in);
    if (!ans.is_open()) {
        throw std::runtime_error("Failed to open file: " + name);
    }

    while (getline(stu, s_stu) && getline(ans, s_ans)) {
        lineno++;
        if (s_stu != s_ans) {
            errstring << "line " << lineno << " mismatch: " << s_stu << "(stu) vs. "
                      << s_ans << "(ans)" << std::endl;
            throw std::runtime_error(errstring.str());
        }
    }

    stu.close();
    ans.close();
}

void grade_test(std::string test_case) {
    SparseMatrix left = SparseMatrix("input/" + test_case + ".left");
    SparseMatrix right = SparseMatrix("input/" + test_case + ".right");

    SparseMatrix ans = left * right;
    ans.to_file("output/" + test_case + ".yourans");

    try {
        compare_res(test_case);
    } catch (std::exception &e) {
        throw std::runtime_error("Failed to pass " + test_case + ":\n\t" +
                                 e.what());
    }
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < num_case; i++) {
        grade_cases[i] = "test" + std::to_string(i + 1);
    }

    try {
        if (argc == 2) {
            grade_test(argv[1]);
        } else if (argc == 1) {
            for (auto test_case : grade_cases) {
                grade_test(test_case);
            }
        } else {
            throw std::runtime_error("[usage]: ./sparsematrix [test_case_name]");
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::cout << "Congratulations! You've passed all test(s)." << std::endl;
    return 0;
}*/
