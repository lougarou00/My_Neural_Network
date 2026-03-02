#include "../../include/Layer.hpp"

Layer::Layer(std::vector<std::unique_ptr<Neuron>> neurons, LayerType type)
    : m_neurons(std::move(neurons)),
        m_nNeurons(m_neurons.size()),
        m_type(type)
{
}

std::vector<std::unique_ptr<Neuron>>& Layer::getNeurons() { 
    return m_neurons; 
}

size_t Layer::getNeuronCount() const {
    return m_nNeurons;
}

LayerType Layer::getType() const {
    return m_type;
}

std::string Layer::layerTypeToString(LayerType type)
{
    switch (type) {
        case LayerType::INPUT:                   return "INPUT";
        case LayerType::HIDDEN:                  return "HIDDEN";
        case LayerType::OUTPUT:                  return "OUTPUT";
        default:                                 return "UNKNOWN";
    }
}

LayerType Layer::StringToLayerType(const std::string& type)
{
    if (type == "INPUT")  return LayerType::INPUT;
    if (type == "HIDDEN") return LayerType::HIDDEN;
    if (type == "OUTPUT") return LayerType::OUTPUT;

    throw std::runtime_error("Unknown layer type: " + type);
}