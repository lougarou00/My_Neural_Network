#include "../../include/Neuron.hpp"

Neuron::Neuron(
    const IActivationFunction& activation,
    int n_weights,
    std::optional<std::vector<float>> weights,
    std::optional<float> bias
) : m_activationfunction(activation)
{
    m_nWeights = n_weights;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1.f, 1.f);

    if (weights) {
        m_weights = std::move(*weights);
        if (m_weights.size() != m_nWeights) {
            throw std::invalid_argument("Size of weights vector does not match n_weights");
        }
    } else {
        m_weights.resize(m_nWeights);
        for (auto& w : m_weights)
            w = dist(gen);
    }

    if (bias) {
        m_bias = *bias;
    } else {
        m_bias = dist(gen);
    }

    m_activation = 0.f;
    m_output = 0.f;
}

float Neuron::compute_z(const std::vector<float>& inputs)
{
    if (inputs.size() != m_weights.size()) {
        throw std::invalid_argument(
            "Input size (" + std::to_string(inputs.size()) + 
            ") does not match number of weights (" + std::to_string(m_weights.size()) + ")"
        );
    }

    float z = m_bias;
    for (size_t i = 0; i < m_weights.size(); ++i) {
        z += m_weights[i] * inputs[i];
    }

    m_z = z;
    return z;
}

float Neuron::compute_activation()
{
    m_activation = m_activationfunction.apply(m_z);
    m_output = m_activation;
    return m_output;
}

float Neuron::get_output()
{
    return m_output;
}

std::vector<float>& Neuron::get_weights()
{
    return m_weights;
}

float& Neuron::get_bias()
{
    return m_bias;
}

float& Neuron::get_z()
{
    return m_z;
}

NeuronType Neuron::getType() const
{
    return NeuronType::SIGMOID;
}

const IActivationFunction& Neuron::get_activation() const
{
    return m_activationfunction;
}

void Neuron::setWeights(const std::vector<float> weights)
{
    m_weights = weights;
    m_nWeights = m_weights.size();
    return;
}

void Neuron::setBias(const float bias)
{
    m_bias = bias;
    return;
}