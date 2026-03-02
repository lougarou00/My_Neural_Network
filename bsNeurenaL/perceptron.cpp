/*
** EPITECH PROJECT, 2025
** bsNeuronalNetwork
** File description:
** exo
*/

#include "perceptron.hpp"
#include <random>

perceptron_t create_perceptron(size_t input_size, float learning_rate)
{
    perceptron_t p;
    p.weights.resize(input_size, 0.0f);
    for (size_t i = 0; i < input_size; ++i) {
        p.weights[i] = static_cast<float>(getRandomFloatOptimized());
    }
    p.bias = 0.0f;
    p.learning_rate = learning_rate;
    return p;
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}


float activate(perceptron_t &p, const std::vector<float> &inputs)
{
    float sum = p.bias;
    for (size_t i = 0; i < inputs.size(); ++i) {
        sum += p.weights[i] * inputs[i];
    }
    return ((float)sigmoid(sum));
}

void train(perceptron_t &p, const std::vector<float> &inputs, float expected_output)
{
    float output = activate(p, inputs);
    float error = expected_output - output;
    for (size_t i = 0; i < inputs.size(); ++i) {
        p.weights[i] += p.learning_rate * error * inputs[i];
    }
    p.bias += p.learning_rate * error;
}

bool test_perceptron(perceptron_t &p, const std::vector<std::pair<std::vector<float>, float>> &test_data)
{
    for (const auto &data : test_data) {
        float output = activate(p, data.first);
        if (output != data.second) {
            return false;
        }
    }
    return true;
}
