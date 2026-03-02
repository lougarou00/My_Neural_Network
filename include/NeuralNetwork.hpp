#pragma once
#include "librairies.hpp"
#include "Layer.hpp"
#include "EvalMetrics.hpp"
#include "Activations.hpp"

class NeuralNetwork {
    public:
        explicit NeuralNetwork(std::vector<Layer> layers);

        NeuralNetwork() {}
    
        size_t getNumLayers() const;
    
        Layer& getLayer(size_t index);
    
        std::vector<float> feedforward(const std::vector<float>& input);
    
        void SGD(std::vector<TrainingSample>& training_data,
                 int epochs,
                 int mini_batch_size,
                 float eta,
                 const std::vector<TrainingSample>* test_data = nullptr);
    
        void updateMiniBatch(const std::vector<TrainingSample>& mini_batch, float eta);
    
        std::pair<std::vector<std::vector<float>>, std::vector<std::vector<std::vector<float>>>>
            backprop(const std::vector<float>& x, const std::vector<float>& y);
    
        EvalMetrics evaluate(const std::vector<TrainingSample>& test_data);

        static std::vector<float> softmax(const std::vector<float>& z);

        void save_config(const std::string& name);

        void save_weights(const std::string& name);

        void save_biases(const std::string& name);

        void save(const std::string& name);

        void setEpochs(const int epochs);

        void setMiniBatchSize(const int mini_batch_size);

        void setLearningRate(const float eta);
    
    private:
        size_t m_numLayers;
        std::vector<Layer> m_layers;
        int m_epochs = 32;
        int m_mini_batch_size = 100;
        float m_eta = 3.0;
    };