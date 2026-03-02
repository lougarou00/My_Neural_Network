#pragma once
#include "librairies.hpp"
#include "IActivationFunction.hpp"

class Neuron {
    public:
        Neuron(
            const IActivationFunction& activation,
            int n_weights,
            std::optional<std::vector<float>> weights = std::nullopt,
            std::optional<float> bias = std::nullopt
        );

        float compute_z(const std::vector<float>& inputs);
    
        float compute_activation();
    
        float get_output();
    
        std::vector<float>& get_weights();
    
        float& get_bias();

        float& get_z();
    
        NeuronType getType() const;

        const IActivationFunction& get_activation() const;

        void setWeights(const std::vector<float> weights);

        void setBias(const float bias);
        
    private:
        size_t m_nWeights;
        std::vector<float> m_weights;
        float m_activation;
        float m_output;
        float m_bias;
        float m_z;
        const IActivationFunction& m_activationfunction;
};