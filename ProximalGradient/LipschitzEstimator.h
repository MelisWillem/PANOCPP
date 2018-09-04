#include"ProximalGradientConfig.h"
#include"IDerivableFunction.h"

#pragma once
class LipschitzEstimator
{
private:
    double _delta_safety_value; // TODO find proper value
    IDerivableFunction* _costFunction;
public:
    LipschitzEstimator(IDerivableFunction & costFunction);
    ~LipschitzEstimator();
    double Estimate(const Vector & location, const Vector & gradient);
};

