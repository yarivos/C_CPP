#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"
#include "Activation.h"

// Insert Dense class here...
class Dense
{
    Matrix _weights, _bias;
    act_func _active_func;
public:
    Dense(const Matrix &weights, const Matrix &bias, act_func active_function)
            : _weights(weights), _bias(bias),
              _active_func(active_function)
    {}


    Matrix get_weights() const
    { return _weights; }

    Matrix get_bias() const
    { return _bias; }

    act_func get_activation() const
    { return _active_func; }

    Matrix operator()(const Matrix &m) const;
};

#endif //DENSE_H
