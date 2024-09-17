#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(const std::string &input_file) {
    std::ifstream in(input_file);
    in >> _row >> _col;

    // 初始化向量列表
    for (int i = 0; i < _row; i++)
        _vector_row.emplace_back(i);
    for (int i = 0; i < _col; i++)
        _vector_col.emplace_back(i);

    // 向列表中填充数值
    int row, col, val;
    while (in >> row) {
        in >> col >> val;
        if (val == 0) continue;
        _vector_row[row].list.emplace_back(node(col, val));
    }
    creat_vector_col();
    in.close();
}

void SparseMatrix::creat_vector_col() {
    for (const auto &i: _vector_row)
        for (auto j: i.list) {
            int row = i.pos, col = j.pos, val = j.value;
            _vector_col[col].list.emplace_back(node(row, val));
        }
}

void SparseMatrix::to_file(const std::string &output_file) const {
    std::ofstream out(output_file);
    out << _row << " " << _col << std::endl;

    for (const auto &i: _vector_row) {
        for (auto j: i.list) {
            out << i.pos << " " << j.pos << " " << j.value << std::endl;
        }
    }
    out.close();
}

SparseMatrix SparseMatrix::operator*(const SparseMatrix &right) {
    SparseMatrix res;
    res._row = _row;
    res._col = right._col;
    for (int i = 0; i < res._row; i++)
        res._vector_row.emplace_back(i);
    for (int i = 0; i < res._col; i++)
        res._vector_col.emplace_back(i);

    // 进行运算
    for (int i = 0; i < _vector_row.size(); i++) {
        for (int j = 0; j < right._vector_col.size(); j++) {
            // 第 i 行 * 第 j 列
            int it_left = 0, it_right = 0, product = 0;
            while (it_left != _vector_row[i].list.size() && it_right != right._vector_col[j].list.size()) {
                int pos_left = _vector_row[i].list[it_left].pos,
                        pos_right = right._vector_col[j].list[it_right].pos,
                        val_left = _vector_row[i].list[it_left].value,
                        val_right = right._vector_col[j].list[it_right].value;
                if (pos_left == pos_right) {
                    product += val_left * val_right;
                    it_left++;
                    it_right++;
                } else if (pos_left < pos_right)
                    it_left++;
                else it_right++;
            }
            if (product) res._vector_row[i].list.emplace_back(j, product);
        }
    }
    // res.creat_vector_col();

    return res;
}