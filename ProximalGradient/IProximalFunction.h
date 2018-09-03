#include"ProximalGradientConfig.h"

#pragma once
class IProximalFunction {
public:
    virtual double Evaluate(const Vector & location, Vector & proxLocation);
};