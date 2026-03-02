#include "../../include/NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(std::vector<Layer> layers)
    : m_numLayers(layers.size()),
        m_layers(std::move(layers))
{
}

size_t NeuralNetwork::getNumLayers() const
{
    return m_numLayers;
}

void NeuralNetwork::setEpochs(const int epochs)
{
    m_epochs = epochs;
    return;
}

void NeuralNetwork::setMiniBatchSize(const int mini_batch_size)
{
    m_mini_batch_size = mini_batch_size;
    return;
}

void NeuralNetwork::setLearningRate(const float eta)
{
    m_eta = eta;
    return;
}

Layer& NeuralNetwork::getLayer(size_t index)
{
    if (index >= m_layers.size())
        throw std::invalid_argument("Index out of range");
    return m_layers[index];
}

std::vector<float> NeuralNetwork::softmax(const std::vector<float>& z)
{
    float max_z = *std::max_element(z.begin(), z.end());

    std::vector<float> result(z.size());
    float sum = 0.f;

    for (size_t i = 0; i < z.size(); ++i) {
        result[i] = std::exp(z[i] - max_z);
        sum += result[i];
    }

    for (float& v : result)
        v /= sum;

    return result;
}

std::vector<float> NeuralNetwork::feedforward(const std::vector<float>& input)
{
    std::vector<float> inputs = input;

    for (size_t i = 1; i < m_numLayers; i++)
    {
        auto& neurons = m_layers[i].getNeurons();
        std::vector<float> z_layer;
        std::vector<float> a_layer;

        for (auto& neuron : neurons) {
            z_layer.push_back(neuron->compute_z(inputs));
        }

        if (i == m_numLayers - 1) {
            a_layer = softmax(z_layer);

            for (size_t n = 0; n < neurons.size(); ++n) {
                neurons[n]->setBias(neurons[n]->get_bias());
                neurons[n]->get_z() = z_layer[n];
                neurons[n]->compute_z(inputs);
            }
        } else {
            for (size_t n = 0; n < neurons.size(); n++) {
                neurons[n]->compute_activation();
                a_layer.push_back(neurons[n]->get_output());
            }
        }
        inputs = a_layer;
    }
    return inputs;
}

void NeuralNetwork::SGD(
    std::vector<TrainingSample>& training_data,
    int epochs,
    int mini_batch_size,
    float eta,
    const std::vector<TrainingSample>* test_data
)
{
    // Saving the parameters of the last training

    m_epochs = epochs;
    m_eta = eta;
    m_mini_batch_size = mini_batch_size;

    const size_t n = training_data.size();
    const size_t n_test = test_data ? test_data->size() : 0;

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int epoch = 0; epoch < epochs; epoch++) {

        std::shuffle(training_data.begin(), training_data.end(), gen);

        for (size_t k = 0; k < n; k += mini_batch_size) {

            size_t end = std::min(k + mini_batch_size, n);

            std::vector<TrainingSample> mini_batch(
                training_data.begin() + k,
                training_data.begin() + end
            );

            updateMiniBatch(mini_batch, eta);

            // Progression displaying

            size_t processed = end; // samples already processed
            float progress = 100.f * processed / n;

            std::cout << "\rEpoch " << epoch + 1 
                    << "/" << epochs 
                    << " - processed " << processed 
                    << "/" << n 
                    << " (" << progress << "%)" 
                    << std::flush;

            std::cout << std::endl; 
        }

        if (test_data) {
            int correct = evaluate(*test_data).accuracy;
            std::cout << "Epoch " << epoch
                      << ": " << correct
                      << " / " << n_test << std::endl;
        }
    }
}

void NeuralNetwork::updateMiniBatch(
    const std::vector<TrainingSample>& mini_batch,
    float eta
)
{
    std::vector<std::vector<std::vector<float>>> nabla_w;
    std::vector<std::vector<float>> nabla_b;

    // Initialization to zero
    for (auto& layer : m_layers) {
        std::vector<std::vector<float>> layer_w;
        std::vector<float> layer_b;

        for (const auto& neuron : layer.getNeurons()) {
            layer_w.emplace_back(neuron->get_weights().size(), 0.0f);
            layer_b.emplace_back(0.0f);
        }

        nabla_w.push_back(std::move(layer_w));
        nabla_b.push_back(std::move(layer_b));
    }

    // Accumulation of gradients for each sample in the mini-batch
    for (const auto& [x, y] : mini_batch) {
        auto gradients = backprop(x, y);

        for (size_t l = 0; l < nabla_w.size(); l++) {
            for (size_t n = 0; n < nabla_w[l].size(); n++) {
                nabla_b[l][n] += gradients.first[l][n];

                for (size_t w = 0; w < nabla_w[l][n].size(); w++) {
                    nabla_w[l][n][w] += gradients.second[l][n][w];
                }
            }
        }
    }

    // Weights and biases update
    const float lr = eta / static_cast<float>(mini_batch.size());

    for (size_t l = 0; l < m_layers.size(); l++) {
        auto& neurons = m_layers[l].getNeurons();

        for (size_t n = 0; n < neurons.size(); n++) {
            auto& weights = neurons[n]->get_weights();
            auto& bias = neurons[n]->get_bias();

            bias -= lr * nabla_b[l][n];

            for (size_t w = 0; w < weights.size(); ++w) {
                weights[w] -= lr * nabla_w[l][n][w];
            }
        }
    }
}

std::pair<
    std::vector<std::vector<float>>,
    std::vector<std::vector<std::vector<float>>>
>
NeuralNetwork::backprop(
    const std::vector<float>& x,
    const std::vector<float>& y
) {
    const size_t num_layers = m_layers.size();

    std::vector<std::vector<float>> nabla_b(num_layers);
    std::vector<std::vector<std::vector<float>>> nabla_w(num_layers);

    // Init gradients
    for (size_t l = 0; l < num_layers; l++) {
        auto& neurons = m_layers[l].getNeurons();
        nabla_b[l].resize(neurons.size(), 0.f);
        nabla_w[l].resize(neurons.size());

        for (size_t n = 0; n < neurons.size(); n++) {
            nabla_w[l][n].resize(neurons[n]->get_weights().size(), 0.f);
        }
    }

    // Feedforward
    std::vector<std::vector<float>> activations;
    std::vector<std::vector<float>> zs;

    activations.push_back(x);
    std::vector<float> current_activation = x;

    for (size_t l = 1; l < num_layers; l++) {
        auto& neurons = m_layers[l].getNeurons();
        std::vector<float> z_layer;
        std::vector<float> a_layer;

        for (auto& neuron : neurons) {
            float z = neuron->compute_z(current_activation);
            z_layer.push_back(z);
            a_layer.push_back(neuron->compute_activation());
        }

        zs.push_back(z_layer);
        activations.push_back(a_layer);
        current_activation = a_layer;
    }

    // Output layer delta
    size_t last = num_layers - 1;
    auto& output_neurons = m_layers[last].getNeurons();
    std::vector<float> delta(output_neurons.size());

    for (size_t i = 0; i < output_neurons.size(); ++i)
    {
        float a = activations[last][i];
    
        delta[i] = a - y[i];
    
        nabla_b[last][i] = delta[i];
    
        for (size_t j = 0; j < activations[last - 1].size(); j++) {
            nabla_w[last][i][j] = delta[i] * activations[last - 1][j];
        }
    }

    // Hidden layers backprop
    for (int l = static_cast<int>(last) - 1; l >= 1; l--) {
        auto& neurons = m_layers[l].getNeurons();
        auto& next_neurons = m_layers[l + 1].getNeurons();

        std::vector<float> new_delta(neurons.size(), 0.f);

        for (size_t i = 0; i < neurons.size(); i++) {
            float sum = 0.f;
            for (size_t k = 0; k < next_neurons.size(); k++) {
                sum += next_neurons[k]->get_weights()[i] * delta[k];
            }

            float z = zs[l - 1][i];
            float sp = neurons[i]->get_activation().derivative(z);

            new_delta[i] = sum * sp;
            nabla_b[l][i] = new_delta[i];

            for (size_t j = 0; j < activations[l - 1].size(); ++j) {
                nabla_w[l][i][j] = new_delta[i] * activations[l - 1][j];
            }
        }

        delta = new_delta;
    }

    return {nabla_b, nabla_w};
}

EvalMetrics NeuralNetwork::evaluate(const std::vector<TrainingSample>& test_data)
{
    size_t num_classes = test_data[0].second.size();
    size_t correct = 0;
    float total_loss = 0.f;

    std::vector<std::vector<size_t>> confusion(num_classes, std::vector<size_t>(num_classes, 0));

    for (const auto& sample : test_data) {
        const std::vector<float>& x = sample.first;
        const std::vector<float>& y = sample.second;

        std::vector<float> output = feedforward(x);

        auto max_it_pred = std::max_element(output.begin(), output.end());
        size_t predicted = std::distance(output.begin(), max_it_pred);

        auto max_it_label = std::max_element(y.begin(), y.end());
        size_t actual = std::distance(y.begin(), max_it_label);

        if (predicted == actual)
            correct++;

        confusion[actual][predicted]++;

        for (size_t i = 0; i < num_classes; ++i) {
            float o = std::max(std::min(output[i], 1.f - 1e-7f), 1e-7f);
            total_loss += -y[i] * std::log(o);
        }
    }

    float accuracy = static_cast<float>(correct) / test_data.size();

    float loss = total_loss / test_data.size();

    std::vector<float> precisions(num_classes, 0.f);
    std::vector<float> recalls(num_classes, 0.f);

    for (size_t i = 0; i < num_classes; i++) {
        size_t TP = confusion[i][i];
        size_t FP = 0, FN = 0;
        for (size_t j = 0; j < num_classes; j++) {
            if (j != i) {
                FP += confusion[j][i];
                FN += confusion[i][j];
            }
        }
        precisions[i] = (TP + FP > 0) ? static_cast<float>(TP) / (TP + FP) : 0.f;
        recalls[i] = (TP + FN > 0) ? static_cast<float>(TP) / (TP + FN) : 0.f;
    }

    float precision = std::accumulate(precisions.begin(), precisions.end(), 0.f) / num_classes;
    float recall = std::accumulate(recalls.begin(), recalls.end(), 0.f) / num_classes;
    float f1_score = (precision + recall > 0.f) ? 2.f * (precision * recall) / (precision + recall) : 0.f;

    return {accuracy, loss, precision, recall, f1_score};
}

void NeuralNetwork::save_config(const std::string& name)
{
    fs::path base_path = fs::path("Neural_Networks") / name;
    fs::create_directories(base_path);

    nlohmann::json j;

    // Training parameters
    j["epochs"] = m_epochs;
    j["mini_batch_size"] = m_mini_batch_size;
    j["learning_rate"] = m_eta;

    // Layers description
    nlohmann::json layers_json = nlohmann::json::array();

    for (size_t i = 1; i < m_layers.size(); i++) {
        Layer& layer = m_layers[i];

        nlohmann::json layer_json;
        layer_json["index"] = i;
        layer_json["layer_type"] = Layer::layerTypeToString(layer.getType());

        // All neurons on the same layer have the same activation

        if (!layer.getNeurons().empty()) {
            const auto& neuron = layer.getNeurons().front();
            layer_json["activation"] =
                activationToString(neuron->get_activation().getType());
            layer_json["number"] = layer.getNeuronCount();
        }
        layers_json.push_back(layer_json);
    }

    j["layers"] = layers_json;

    std::ofstream file(base_path / (name + ".json"));
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file to save config");
    }

    file << j.dump(4);
    file.close();
}

void NeuralNetwork::save_weights(const std::string& name)
{
    fs::path weights_path = fs::path("Neural_Networks") / name / "weights";
    fs::create_directories(weights_path);

    // Delete old files
    for (auto& p : fs::directory_iterator(weights_path)) {
        fs::remove(p);
    }

    for (size_t l = 1; l < m_layers.size(); l++) {
        std::ofstream ofs(weights_path / ("weights-" + std::to_string(l) + ".bin"), std::ios::binary);
        if (!ofs.is_open())
            throw std::runtime_error("Cannot open weight file");

        auto& neurons = m_layers[l].getNeurons();
        for (auto& neuron : neurons) {
            auto& w = neuron->get_weights();
            int n_weights = static_cast<int>(w.size());
            ofs.write(reinterpret_cast<char*>(&n_weights), sizeof(int));
            ofs.write(reinterpret_cast<char*>(w.data()), n_weights * sizeof(float));
        }
        ofs.close();
    }
}

void NeuralNetwork::save_biases(const std::string& name)
{
    fs::path biases_path = fs::path("Neural_Networks") / name / "biases";
    fs::create_directories(biases_path);

    // Delete old files
    for (auto& p : fs::directory_iterator(biases_path)) {
        fs::remove(p);
    }

    for (size_t l = 1; l < m_layers.size(); l++) {
        std::ofstream ofs(biases_path / ("biases-" + std::to_string(l) + ".bin"), std::ios::binary);
        if (!ofs.is_open())
            throw std::runtime_error("Cannot open bias file");

        auto& neurons = m_layers[l].getNeurons();
        for (auto& neuron : neurons) {
            float b = neuron->get_bias();
            ofs.write(reinterpret_cast<char*>(&b), sizeof(float));
        }
        ofs.close();
    }
}

void NeuralNetwork::save(const std::string& name)
{
    save_config(name);
    save_weights(name);
    save_biases(name);
}

extern "C" NeuralNetwork* createInstance() {
    return new NeuralNetwork();
}