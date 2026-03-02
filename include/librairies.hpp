#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <optional>
#include <random>
#include <memory>
#include <nlohmann/json.hpp>
#include <dlfcn.h>
#include <stdexcept>
#include <string>

enum class NeuronType {
    PERCEPTRON,
    SIGMOID,
};

enum class ActivationFunctionType {
    STEP,
    SIGMOID,
    RELU,
    TANH
};

enum class LayerType {
    INPUT,
    HIDDEN,
    OUTPUT
};

using TrainingSample = std::pair<std::vector<float>, std::vector<float>>;
namespace fs = std::filesystem;
