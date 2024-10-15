#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

typedef Matrix &(*act_func)(Matrix &m);
// Insert Activation class here...
namespace activation
{
    Matrix &relu(Matrix &m);

    Matrix &softmax(Matrix &m);
}
#endif //ACTIVATION_H