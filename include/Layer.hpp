#pragma once
#include "librairies.hpp"
#include "Neuron.hpp"

class Layer {
    public:
        explicit Layer(std::vector<std::unique_ptr<Neuron>> neurons, LayerType type);
    
        std::vector<std::unique_ptr<Neuron>>& getNeurons();
    
        size_t getNeuronCount() const;
    
        LayerType getType() const;

        static std::string layerTypeToString(LayerType type);

        static LayerType StringToLayerType(const std::string& type);
    
    private:
        std::vector<std::unique_ptr<Neuron>> m_neurons;
        size_t m_nNeurons;
        LayerType m_type;
    };

