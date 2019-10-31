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
    IProximalFunction* _constraint;

    double _linesearchGamma=0;
    Vector* _proximalStep;

    int _maxNumberOfLinesearches = 10;

    bool LinesearchCondition(const Vector& location);
    void CalculateProximalStep(
        const Vector& location,
        const Vector & gradient);
public:
    ProximalCalculator(
        const IDerivableFunction costFunction,
        const IProximalFunction constraint);
    ~ProximalCalculator();

    void Calculate(const Vector& location);

    //getters called after calculate method
    Vector* GetProximalStep();
    void GetResidual(
        const Vector & location, 
        Vector& residual);
};

