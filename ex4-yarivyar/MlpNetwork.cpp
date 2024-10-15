#include "MlpNetwork.h"

#define BASE_10 10
//MlpNetwork::MlpNetwork(Matrix *weights, Matrix *biases) : layer1(weights[0], biases[0], activation::relu),
//                                                          layer2(weights[1], biases[1], activation::relu),
//                                                          layer3(weights[2], biases[2], activation::relu),
//                                                          layer4(weights[3], biases[3], activation::softmax) {}

MlpNetwork::MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE])
{
    for (int i = 0; i < MLP_SIZE; i++)
    {
        _weights[i] = weights[i];
        _biases[i] = biases[i];
    }
}

digit MlpNetwork::operator()(Matrix &m)
{
    Dense layer1 = Dense(_weights[0], _biases[0], activation::relu);
    Dense layer2 = Dense(_weights[1], _biases[1], activation::relu);
    Dense layer3 = Dense(_weights[2], _biases[2], activation::relu);
    Dense layer4 = Dense(_weights[3], _biases[3], activation::softmax);
    Matrix new_mat = layer4(layer3(layer2(layer1(m))));
    unsigned int number = 0;
    float prob = new_mat[0];
    float sum = new_mat[0];
    for (int i = 1; i < BASE_10; i++)
    {
        sum += new_mat[i];
        if (prob < new_mat[i])
        {
            prob = new_mat[i];
            number = i;
        }
    }
    return digit{number, prob};
}
