#include "../../include/NeuronFactory.hpp"

std::unique_ptr<Neuron> NeuronFactory::create(
    const std::string& activation,
    size_t n_weights,
    std::optional<std::vector<float>> weights,
    std::optional<float> bias
)
{
    const IActivationFunction* act_ptr = nullptr;

    if (activation == "SIGMOID") act_ptr = &Activations::sigmoid;
    else if (activation == "RELU") act_ptr = &Activations::relu;
    else if (activation == "TANH") act_ptr = &Activations::tanh;
    else if (activation == "PERCEPTRON") act_ptr = &Activations::step;
    else throw std::runtime_error("Unknown activation: " + activation);

    return std::make_unique<Neuron>(*act_ptr, n_weights, weights, bias);
}
