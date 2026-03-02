#pragma once
#include "librairies.hpp"
#include "IActivationFunction.hpp"
#include "SigmoidActivationFunction.hpp"
#include "RELUActivationFunction.hpp"
#include "TANHActivationFunction.hpp"
#include "StepActivationFunction.hpp"

namespace Activations {
    inline SigmoidActivationFunction sigmoid;
    inline TANHActivationFunction tanh;
    inline RELUActivationFunction relu;
    inline StepActivationFunction step;
}

inline std::string activationToString(ActivationFunctionType type)
{
    switch (type) {
        case ActivationFunctionType::SIGMOID:    return "SIGMOID";
        case ActivationFunctionType::RELU:       return "RELU";
        case ActivationFunctionType::TANH:       return "TANH";
        case ActivationFunctionType::STEP:       return "PERCEPTRON";
        default:                                 return "UNKNOWN";
    }
}
