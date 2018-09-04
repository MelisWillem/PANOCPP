#include"ProximalGradientConfig.h"

#pragma once
class IDerivableFunction {
    public:
        virtual double Evaluate(const Vector& location, Vector& gradient);
        virtual unsigned int GetDimension();
};