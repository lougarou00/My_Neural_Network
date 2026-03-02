#pragma once
#include "librairies.hpp"

class IActivationFunction {
    public:
        virtual float apply(const float z) const = 0;
        virtual ActivationFunctionType getType() const = 0;
        virtual float derivative(float z) const = 0;
};
