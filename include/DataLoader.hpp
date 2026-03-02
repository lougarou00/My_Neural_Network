#pragma once
#include "librairies.hpp"

class DataLoader {
    public:
        static std::vector<TrainingSample> loadData(const std::string& filename);
        static std::vector<std::vector<float>> load_weights(const std::string& path);
        static std::vector<float> load_biases(const std::string& path);
};