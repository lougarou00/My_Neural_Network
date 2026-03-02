#include "../../include/ExperimentRunner.hpp"

NeuralNetwork ExperimentRunner::generate(const std::string& config_file)
{
    // Read the JSON
    nlohmann::json j;
    {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config file: " + config_file);
        }
        file >> j;
    }

    const auto& layers_json = j.at("layers");
    std::vector<Layer> layers_vector;

    // --- Step 1: Create dummy input layer ---
    std::vector<std::unique_ptr<Neuron>> input_neurons;
    input_neurons.push_back(
        std::make_unique<Neuron>(
            StepActivationFunction{},
            1,
            std::vector<float>{1.0f},
            0.0f
        )
    );
    layers_vector.push_back(Layer(std::move(input_neurons), LayerType::INPUT));

    size_t previous_size = 65;

    // --- Create hidden layers ---
    for (size_t l = 0; l < layers_json.size() - 1; l++) {
        const auto& layer = layers_json[l];

        std::string layer_type = layer.at("type").get<std::string>();
        std::string neuron_type = layer.at("neuron_type").get<std::string>();
        size_t layer_size = layer.at("size").get<size_t>();

        std::vector<std::unique_ptr<Neuron>> neurons_vector;
        for (size_t n = 0; n < layer_size; n++) {
            neurons_vector.push_back(
                NeuronFactory::create(neuron_type, previous_size)
            );
        }

        previous_size = layer_size;
        layers_vector.push_back(Layer(std::move(neurons_vector), LayerType::HIDDEN));
    }

    // --- Create output layer with 5 neurons ---
    //const auto& layer = layers_json.back(); // output layer
    std::string neuron_type = "SIGMOID";
    size_t output_size = 5;

    std::vector<std::unique_ptr<Neuron>> output_neurons;
    for (size_t n = 0; n < output_size; ++n) {
        output_neurons.push_back(
            NeuronFactory::create(neuron_type, previous_size)
        );
    }

    layers_vector.push_back(Layer(std::move(output_neurons), LayerType::OUTPUT));
    return NeuralNetwork(std::move(layers_vector));
}

void ExperimentRunner::saveNetwork(NeuralNetwork &network, const std::string &name)
{
    network.save(name);
    return;
}

NeuralNetwork ExperimentRunner::loadNetwork(const std::string& network_name)
{
    fs::path base_path = fs::path("Neural_Networks") / network_name;
    fs::path config_path = base_path / (network_name + ".json");

    if (!fs::exists(config_path)) {
        throw std::runtime_error("Network config not found: " + config_path.string());
    }

    // Load JSON config
    nlohmann::json j;
    {
        std::ifstream file(config_path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open network config file");
        }
        file >> j;
    }

    int epochs = j.at("epochs").get<int>();
    int mini_batch_size = j.at("mini_batch_size").get<int>();
    float learning_rate = j.at("learning_rate").get<float>();

    const auto& layers_json = j.at("layers");

    fs::path weights_dir = base_path / "weights";
    fs::path biases_dir  = base_path / "biases";

    std::vector<Layer> layers_vector;

    // ----------------------------
    // Create input layer automatically
    // ----------------------------
    size_t input_size = 1;

    std::vector<std::unique_ptr<Neuron>> input_neurons;
    for (size_t n = 0; n < input_size; n++) {
        input_neurons.push_back(
            NeuronFactory::create(
                "PERCEPTRON",
                1,
                std::vector<float>{1.f},
                0.f
            )
        );
    }
    layers_vector.emplace_back(std::move(input_neurons), LayerType::INPUT);

    // ----------------------------
    // Rebuild hidden and output layers from JSON
    // ----------------------------
    for (size_t idx = 0; idx < layers_json.size(); idx++) {
        const auto& layer_json = layers_json[idx];

        std::string layer_type_str  = layer_json.at("layer_type").get<std::string>();
        std::string activation_name = layer_json.at("activation").get<std::string>();
        size_t number_neurons       = layer_json.at("number").get<size_t>();

        LayerType layer_type = Layer::StringToLayerType(layer_type_str);

        std::vector<std::unique_ptr<Neuron>> neurons_vector;

        fs::path w_file = weights_dir / ("weights-" + std::to_string(idx + 1) + ".bin");
        fs::path b_file = biases_dir  / ("biases-"  + std::to_string(idx + 1) + ".bin");

        auto weights = DataLoader::load_weights(w_file.string());
        auto biases  = DataLoader::load_biases(b_file.string());

        if (weights.size() != number_neurons ||
            biases.size()  != number_neurons) {
            throw std::runtime_error("Weights/Biases size mismatch in layer " + std::to_string(idx));
        }

        for (size_t n = 0; n < number_neurons; n++) {
            neurons_vector.push_back(
                NeuronFactory::create(
                    activation_name,
                    weights[n].size(),
                    weights[n],
                    biases[n]
                )
            );
        }

        layers_vector.emplace_back(std::move(neurons_vector), layer_type);
    }

    NeuralNetwork net(std::move(layers_vector));
    
    net.setEpochs(epochs);
    net.setMiniBatchSize(mini_batch_size);
    net.setLearningRate(learning_rate);

    return net;
}

void ExperimentRunner::train(
    NeuralNetwork& network,
    const std::string& hyperparams_file,
    const std::string& training_data_file
)
{
    // Load hyperparameters from JSON
    nlohmann::json j;
    {
        std::ifstream file(hyperparams_file);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open hyperparameters file: " + hyperparams_file);
        }
        file >> j;
    }

    int epochs = j.at("epochs").get<int>();
    int mini_batch_size = j.at("mini_batch_size").get<int>();
    float eta = j.at("eta").get<float>();

    // Load training data
    DataLoader loader;
    std::vector<TrainingSample> training_data = loader.loadData(training_data_file);

    // Train the model
    network.SGD(training_data, epochs, mini_batch_size, eta);
    return;
}

void ExperimentRunner::writeBenchmarkData(EvalMetrics metrics, size_t total_samples)
{
    std::ofstream file("benchmark_value/benchmark_data.txt", std::ios::app);
    if (file.is_open()) {
        file << std::fixed << std::setprecision(4);
        file << "total_samples: " << total_samples << "\n";
        file << "accuracy: " << metrics.accuracy << "\n";
        file << "loss: " << metrics.loss << "\n";
        file << "precision: " << metrics.precision << "\n";
        file << "recall: " << metrics.recall << "\n";
        file << "f1_score: " << metrics.f1_score << "\n";
        file << "----\n";
        file.close();
    } else {
        std::cerr << "Error: Could not open file for writing" << std::endl;
    }
}

void ExperimentRunner::evaluate(NeuralNetwork& network, const std::string& test_file)
{
    // Load the training dataset
    std::vector<TrainingSample> test_data = DataLoader::loadData(test_file);

    // Total number of samples
    size_t n = test_data.size();

    // Compute metrics
    EvalMetrics metrics = network.evaluate(test_data);

    // Displaying results

    std::cout << "Evaluation Results:" << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "Total samples: " << n << std::endl;
    std::cout << "Accuracy: " << metrics.accuracy << std::endl;
    std::cout << "Loss: " << metrics.loss << std::endl;
    std::cout << "Precision: " << metrics.precision << std::endl;
    std::cout << "Recall: " << metrics.recall << std::endl;
    std::cout << "F1 Score: " << metrics.f1_score << std::endl;
    std::cout << test_file << std::endl;
    writeBenchmarkData(metrics, n);
    return;
}

void ExperimentRunner::predict(NeuralNetwork& network, const std::string& test_file)
{
    std::ifstream file(test_file);
    if (!file.is_open()) {
        exit(84);
        throw std::runtime_error("Unable to open test file: " + test_file);
    }

    constexpr size_t INPUT_SIZE = 65;

    // Map output index -> game state string
    std::map<int, std::string> outputMap = {
        {0, "Check White"},
        {1, "Check Black"},
        {2, "Checkmate White"},
        {3, "Checkmate Black"},
        {4, "Nothing"}
    };

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        // Retrieve all 73 values
        std::vector<float> values = Convertor::retrieveBoardInfo(line);
        if (values.size() < INPUT_SIZE) {
            std::cerr << "Warning: line skipped, expected at least 65 values, got "
                      << values.size() << "\n";
            continue;
        }

        // Take first 65 values as input
        std::vector<float> input(values.begin(), values.begin() + INPUT_SIZE);

        // Feedforward
        std::vector<float> output = network.feedforward(input);

        // Find max index
        int maxIndex = 0;
        float maxVal = output[0];
        for (size_t i = 1; i < output.size(); i++) {
            if (output[i] > maxVal) {
                maxVal = output[i];
                maxIndex = static_cast<int>(i);
            }
        }
        // Display predicted game state
        std::cout << (outputMap.count(maxIndex) ? outputMap[maxIndex] : "UNKNOWN")
                  << "\n";
    }

    file.close();
}

extern "C" ExperimentRunner* createInstance() {
    return new ExperimentRunner();
}