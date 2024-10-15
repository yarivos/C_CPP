#include "Activation.h"
#include <cmath>


Matrix &activation::relu(Matrix &m)
{
    int mat_length = m.Matrix::get_rows() * m.Matrix::get_cols();
    for (int i = 0; i < mat_length; i++)
    {
        if (m[i] < 0)
        {
            m[i] = 0;
        }
    }
    return m;
}

Matrix &activation::softmax(Matrix &m)
{
    float sum_exp = 0;
    int mat_length = m.Matrix::get_rows() * m.Matrix::get_cols();
    for (int i = 0; i < mat_length; i++)
    {
        m[i] = std::exp(m[i]);
        sum_exp += m[i];
    }
    m = m * (1 / sum_exp);
    return m;
}