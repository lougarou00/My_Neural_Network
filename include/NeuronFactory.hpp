#pragma once
#include "librairies.hpp"
#include "Neuron.hpp"
#include "Activations.hpp"

class NeuronFactory {
    public:
        static std::unique_ptr<Neuron> create(
            const std::string& activation,
            size_t n_weights,
            std::optional<std::vector<float>> weights = std::nullopt,
            std::optional<float> bias = std::nullopt
        );
};
