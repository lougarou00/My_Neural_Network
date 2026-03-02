#pragma once
#include "librairies.hpp"
#include "IActivationFunction.hpp"

class SigmoidActivationFunction : public IActivationFunction {
    public:
        SigmoidActivationFunction() = default;

        float apply(const float z) const override {
            return  1.0f / (1.0f + std::exp(-z));
        }

        ActivationFunctionType getType() const override {
            return ActivationFunctionType::SIGMOID;
        }

        float derivative(float z) const override {
            float s = apply(z);
            return s * (1.f - s);
        }
};