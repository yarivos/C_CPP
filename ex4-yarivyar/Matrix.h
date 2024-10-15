// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...
class Matrix
{
    float *_data = nullptr;
    int _rows, _cols, _length;
public:
    Matrix(int rows, int cols);

    Matrix() : Matrix(1, 1)
    {}

    Matrix(const Matrix &m);

    ~Matrix();

    // ************************* //
    //         METHODS           //
    // ************************* //
    int get_rows() const
    { return _rows; }

    int get_cols() const
    { return _cols; }

    /// Transforms a matrix into its transpose matrix,
    /// Supports function calling concatenation.
    Matrix &transpose();

    /// Transforms a matrix into a column vector.
    ///Supports function calling concatenation.
    Matrix &vectorize();

    ///Prints matrix elements, no return value.
    ///Prints space after each element (including last
    ///element in row).
    ///Prints newline after each row (including last row).
    void plain_print() const;

    /// Returns a matrix which is the elementwise multiplication(Hadamard product) of this matrix and
    ///another matrix m:
    /// \param m
    /// \return copy of new matrix after dot product
    Matrix dot(Matrix &m) const;

    /// Returns the Frobenius norm of the given
    /// \return
    float norm() const;

    // ************************* //
    //         OPERATORS         //
    // ************************* //
    Matrix operator+(const Matrix &rhs) const;

    Matrix &operator=(const Matrix &rhs);

    Matrix operator*(const Matrix &rhs) const;

    Matrix operator*(float c) const;

    friend Matrix operator*(float c, const Matrix &rhs);

    Matrix &operator+=(const Matrix &rhs);

    float &operator()(int row, int col) const;

    float &operator[](int i) const;

    friend std::ostream &operator<<(std::ostream &s, const Matrix &rhs);

    friend std::istream &operator>>(std::ifstream &s, Matrix &rhs);


};


#endif //MATRIX_H