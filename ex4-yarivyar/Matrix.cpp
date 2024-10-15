#include "Matrix.h"
#include <iostream>
#include <cmath>
#include <fstream>

#define PRINT_BORDER 0.1

Matrix::Matrix(int rows, int cols) {
    _rows = rows;
    _cols = cols;
    _length = rows * cols;
    _data = new float[_length];
    for (int i = 0; i < _length; i++) {
        _data[i] = 0.0;
    }
}


Matrix::Matrix(const Matrix &m) {
    this->_rows = m._rows;
    this->_cols = m._cols;
    this->_length = m._rows * m._cols;
    this->_data = new float[m._rows * m._cols];
    // finished initializing
    for (int i = 0; i < _length; i++) // assigning values
    {
        _data[i] = m._data[i];
    }
}

Matrix::~Matrix() {
    delete[] _data;
}

Matrix &Matrix::transpose() {
    float temp;
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            temp = (*this)(row, col);
            (*this)(row, col) = (*this)(col, row);
            (*this)(col, row) = temp;
        }
    }
    int temp2 = _cols;
    _cols = _rows;
    _rows = temp2;
    return *this;
}

Matrix &Matrix::vectorize() {
    _rows = _length;
    _cols = 1;
    return *this;
}

void Matrix::plain_print() const {
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            std::cout << (*this)(row, col) << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::dot(Matrix &rhs) const {
    if (_rows != rhs._rows || _cols != rhs._cols) {
        throw std::length_error("Matrix Sizes are Different");
    }
    Matrix new_matrix = Matrix(_rows, _cols);
    for (int i = 0; i < _length; i++) {
        new_matrix._data[i] = _data[i] * rhs._data[i];
    }
    return new_matrix;
}

float Matrix::norm() const {
    float norm = 0;
    for (int i = 0; i < _length; i++) {
        norm += _data[i] * _data[i];
    }
    norm = (float) std::sqrt(norm);
    return norm;
}

Matrix Matrix::operator+(const Matrix &rhs) const {
    if (_rows != rhs._rows || _cols != rhs._cols) {
        throw std::length_error("Matrix Sizes are Different 2");
    }
    Matrix new_matrix = Matrix(_rows, _cols);
    for (int i = 0; i < _length; i++) {

        new_matrix._data[i] = _data[i] + rhs._data[i];
    }
    return new_matrix;
}

Matrix &Matrix::operator=(const Matrix &rhs) {
    if (this == &rhs) {
        return *this;
    }
    delete[] _data;
    this->_rows = rhs._rows;
    this->_cols = rhs._cols;
    this->_length = rhs._length;
    this->_data = new float[_length];
    for (int i = 0; i < _length; i++) // assigning values
    {
        _data[i] = rhs._data[i];
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix &rhs) const {
    if (_cols != rhs._rows) {
        throw std::length_error("Matrix Sizes are Different 3");
    }
    Matrix new_matrix = Matrix(_rows, rhs._cols);
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < rhs._cols; col++) {
            for (int k = 0; k < _cols; k++) // assigning values
            {
                new_matrix(row, col) += (*this)(row, k) * rhs(k, col);
            }
        }
    }
    return new_matrix;
}

Matrix Matrix::operator*(const float c) const {
    Matrix new_matrix = Matrix(_rows, _cols);
    for (int i = 0; i < _length; i++) // assigning values
    {
        new_matrix._data[i] = _data[i] * c;
    }
    return new_matrix;
}

Matrix operator*(const float c, const Matrix &rhs) {
    Matrix new_matrix = Matrix(rhs._rows, rhs._cols);
    for (int i = 0; i < rhs._length; i++) // assigning values
    {
        new_matrix._data[i] = rhs._data[i] * c;
    }
    return new_matrix;
}

Matrix &Matrix::operator+=(const Matrix &rhs) {
    if (_rows != rhs._rows || _cols != rhs._cols) {
        throw std::length_error("Matrix Sizes are Different 4");
    }
    for (int i = 0; i < _length; i++) // assigning values
    {
        _data[i] += rhs._data[i];
    }
    return *this;
}

float &Matrix::operator()(const int row, const int col) const {
    if (row < 0 || col < 0) {
        throw std::out_of_range("Matrix Sizes are Different 5");
    }
    return _data[row * _cols + col];
}

float &Matrix::operator[](const int i) const {
    if (i > _length || i < 0) {
        throw std::out_of_range("Matrix Sizes are Different 6");
    }
    return _data[i];
}

std::ostream &operator<<(std::ostream &s, const Matrix &rhs) {
    for (int row = 0; row < rhs._rows; row++) {
        for (int col = 0; col < rhs._cols; col++) {
            if (rhs(row, col) > PRINT_BORDER) {
                s << "**";
            } else {
                s << "  ";
            }
        }
        s << "\n";
    }
    return s;
}

std::istream &operator>>(std::ifstream &s, Matrix &rhs) {
    std::streamsize streamsize = rhs.get_cols() * rhs.get_rows() * sizeof(float);
    s.read((char *) rhs._data, streamsize);
    return s;
}

