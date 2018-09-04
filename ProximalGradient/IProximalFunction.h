#include"ProximalGradientConfig.h"

#pragma once
class IProximalFunction {
public:
    virtual double Evaluate(Vector & location);
};