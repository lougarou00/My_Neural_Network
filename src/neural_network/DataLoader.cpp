#include "../../include/DataLoader.hpp"

std::vector<TrainingSample> DataLoader::loadData(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open data file: " + filename);
    }

    std::vector<TrainingSample> dataset;
    std::string line;

    constexpr size_t TOTAL_VALUES = 78;
    constexpr size_t OUTPUT_SIZE  = 5;
    constexpr size_t INPUT_SIZE   = 65;

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::vector<float> values;
        float v;

        while (ss >> v) {
            values.push_back(v);
        }

        if (values.size() != TOTAL_VALUES) {
            throw std::runtime_error(
                "Invalid data line: expected 78 values, got " +
                std::to_string(values.size())
            );
        }

        std::vector<float> input;
        input.reserve(INPUT_SIZE);

        // Board squares (64)
        input.insert(input.end(), values.begin(), values.begin() + 64);

        // Side to move (1)
        input.push_back(values[64]);

        // Build OUTPUT (5 values)
        std::vector<float> output(
            values.end() - OUTPUT_SIZE,
            values.end()
        );

        dataset.emplace_back(std::move(input), std::move(output));
    }
    return dataset;
}

std::vector<std::vector<float>> DataLoader::load_weights(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open weights file: " + path);
    }

    std::vector<std::vector<float>> weights;

    while (true) {
        int n_weights = 0;

        // Read the weight of each neuron
        file.read(reinterpret_cast<char*>(&n_weights), sizeof(int));

        // Proper end file
        if (file.eof()) {
            break;
        }

        if (!file || n_weights <= 0) {
            throw std::runtime_error("Corrupted weights file: " + path);
        }

        std::vector<float> neuron_weights(n_weights);
        file.read(reinterpret_cast<char*>(neuron_weights.data()),
                  n_weights * sizeof(float));

        if (!file) {
            throw std::runtime_error("Corrupted weights file (weights read): " + path);
        }

        weights.push_back(std::move(neuron_weights));
    }

    file.close();
    return weights;
}

std::vector<float> DataLoader::load_biases(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open biases file: " + path);
    }

    std::vector<float> biases;
    float bias;

    while (file.read(reinterpret_cast<char*>(&bias), sizeof(float))) {
        biases.push_back(bias);
    }

    if (!file.eof()) {
        throw std::runtime_error("Corrupted biases file: " + path);
    }

    file.close();
    return biases;
}
