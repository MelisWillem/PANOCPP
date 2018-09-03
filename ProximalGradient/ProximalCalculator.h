#include "LipschitzEstimator.h"
#include "IDerivableFunction.h"
#include "IProximalFunction.h"
#include "ProximalGradientConfig.h"

#pragma once
class ProximalCalculator
{
private:
    LipschitzEstimator* _estimator;
    IDerivableFunction* _costFunction;

    Vector proximalStep;
    double _forwardBackwardEnvelope;
public:
    ProximalCalculator();
    ~ProximalCalculator();
    void Calculate(
        const IDerivableFunction costFunction,
        const IProximalFunction constraint);
    double GetFBELastCalculations();
};

