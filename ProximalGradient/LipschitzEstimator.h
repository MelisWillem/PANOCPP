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
    double Estimate(Vector & location, Vector & gradient);
};

