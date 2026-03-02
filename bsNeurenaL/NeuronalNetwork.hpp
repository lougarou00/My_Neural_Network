/*
** EPITECH PROJECT, 2025
** bsNeuronalNetwork
** File description:
** exo
*/

#pragma once
#include "perceptron.hpp"

class Layer {
    private:
        std::vector<perceptron_t> _perceptrons;
        float _learning_rate;
        size_t _size;
        std::vector<float> _outputs;
        std::vector<float> _delta_layer;
        float ponderedDelta(const std::vector<perceptron_t> &perceptrons, const std::vector<float> &delta_layer, size_t size, size_t index)
        {
            float sum = 0.0f;
            for (size_t i = 0; i < size; ++i) {
                sum += perceptrons[i].weights[index] * delta_layer[i];
            }
            return sum;
        }
    public:
        Layer(size_t input_size, size_t layer_size, float learning_rate)
        {
            _learning_rate = learning_rate;
            _size = layer_size;
            _outputs.resize(layer_size, 0.0f);
            _delta_layer.resize(layer_size, 0.0f);
            for (size_t i = 0; i < layer_size; ++i) {
                _perceptrons.push_back(create_perceptron(input_size, learning_rate));
            }
        }
        std::vector<perceptron_t> getPerceptrons() const
        {
            return _perceptrons;
        }
        std::vector<float> getDeltaLayer() const
        {
            return _delta_layer;
        }
        void activateLayer(const std::vector<float> &inputs)
        {
            for (int a = 0; a < _size; a++) {
                _outputs[a] = activate(_perceptrons[a], inputs);
            }
        }

        float getSize() const
        {
            return _size;
        }
        std::vector<float> getOutputs() const
        {
            return _outputs;
        }

        void traninLayer(const std::vector<float> &inputs, std::vector<perceptron_t> &perceptrons, std::vector<float> &delta_layer, size_t size)
        {
            for (int a = 0; a < _size; a++) {
                float pond = ponderedDelta(perceptrons, delta_layer, size, a);
                float temp = pond * _outputs[a] * (1 - _outputs[a]);
                _delta_layer[a] =  temp;
                for (int b = 0; b < inputs.size(); b++) {
                    _perceptrons[a].weights[b] += _learning_rate * _delta_layer[a] * inputs[b];
                }
                _perceptrons[a].bias = _learning_rate * _delta_layer[a];
            }
        }
};

class NeuralNetwork {
private:

    size_t _input_size;
    float _learning_rate;
    perceptron_t _output_perceptron;
    std::vector<Layer> _hiddern_layers;
    bool isOutputSet = false;
public:

    NeuralNetwork(size_t input_size, float learning_rate) : _input_size(input_size), _learning_rate(learning_rate)
    {}

    void addLayer(size_t layer_size)
    {
        size_t prev_size = _input_size;
        if (!_hiddern_layers.empty()) {
            prev_size = _hiddern_layers.back().getSize();
        }
        _hiddern_layers.push_back(Layer(prev_size, layer_size, _learning_rate));
    }

    float activateNeurone(const std::vector<float> &inputs)
    {
        std::vector<float> current_inputs = inputs;
        for (auto &layer : _hiddern_layers) {
            layer.activateLayer(current_inputs);
            current_inputs = layer.getOutputs();
        }
        if (!isOutputSet) {
            _output_perceptron = create_perceptron(current_inputs.size(), _learning_rate);
            isOutputSet = true;
        }
        return activate(_output_perceptron, current_inputs);
    }
    void back_propagation(const std::vector<float> &inputs, float expected_output)
    {
        float output = activateNeurone(inputs);
        float error = expected_output - output;
        float delta_output = error * output * (1 - output);
        for (int a = 0; a < _output_perceptron.weights.size(); a++) {
            _output_perceptron.weights[a] += _learning_rate * delta_output * _hiddern_layers.back().getOutputs()[a];
        }
        _output_perceptron.bias += _learning_rate * delta_output;
        std::vector<perceptron_t> next_perceptrons = {_output_perceptron};
        std::vector<float> next_delta_layer = {delta_output};
        size_t size = 1;
        for (int i = _hiddern_layers.size() - 1; i >= 0; i--) {
            std::vector<float> layer_inputs;
            if (i == 0) {
                layer_inputs = inputs;
            } else {
                layer_inputs = _hiddern_layers[i - 1].getOutputs();
            }
            _hiddern_layers[i].traninLayer(layer_inputs, next_perceptrons, next_delta_layer, size);
            next_perceptrons = _hiddern_layers[i].getPerceptrons();
            next_delta_layer = _hiddern_layers[i].getDeltaLayer();
            size = _hiddern_layers[i].getSize();
        }
    }
};
