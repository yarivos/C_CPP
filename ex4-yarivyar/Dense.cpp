#include "Dense.h"

Matrix Dense::operator()(const Matrix &m) const
{
    Matrix mult = m;
    mult = (this->get_weights() * m);
    mult += this->get_bias();
    this->get_activation()(mult);
    return mult;
}
