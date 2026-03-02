#pragma once
#include "librairies.hpp"
#include "IActivationFunction.hpp"

class TANHActivationFunction : public IActivationFunction {
    public:

        TANHActivationFunction() = default;

        float apply(const float z) const override {
            return std::tanh(z);
        }

        ActivationFunctionType getType() const override {
            return ActivationFunctionType::TANH;
        }

        float derivative(float z) const override {
            float t = std::tanh(z);
            return 1.f - t * t;
        }
};
