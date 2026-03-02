#pragma once
#include <vector>
#include <cstddef>
#include <random>
#include <iostream>
#include <cmath>

typedef struct perceptron
{
    std::vector<float> weights;
    float bias;
    float learning_rate;
} perceptron_t;

float activate(perceptron_t &p, const std::vector<float> &inputs);
void train(perceptron_t &p, const std::vector<float> &inputs, float expected_output);
int getRandomInt(int min, int max);
bool test_perceptron(perceptron_t &p, const std::vector<std::pair<std::vector<float>, float>> &test_data);
float computeXor(float a, float b);
double sigmoid(double x);
double getRandomFloatOptimized();
perceptron_t create_perceptron(size_t input_size, float learning_rate);
