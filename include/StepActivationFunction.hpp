#pragma once
#include "librairies.hpp"
#include "IActivationFunction.hpp"

class StepActivationFunction : public IActivationFunction {
    public:
        StepActivationFunction() = default;

        float apply(const float z) const override {
            return (z >= 0) ? 1.0f : 0.0f;
        }

        ActivationFunctionType getType() const override {
            return ActivationFunctionType::STEP;
        }

        float derivative(float) const override {
            return 0.f; // no gradient learning
        }
};