#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

struct node {
    int pos;
    int value;

    explicit node(int n = 0, int v = 0) {
        pos = n;
        value = v;
    }
};

struct data_list {
    int pos;
    std::vector <node> list;

    explicit data_list(int n = 0) {
        pos = n;
    }
};

class SparseMatrix {
private:
    int _row = 0;
    int _col = 0;

    std::vector <data_list> _vector_row, _vector_col;

public:
    SparseMatrix() = default;

    explicit SparseMatrix(const std::string &input_file);

    void creat_vector_col();

    void to_file(const std::string &output_file) const;

    SparseMatrix operator*(const SparseMatrix &right);
};