#pragma once

#include <vector>
#include "LipschitzEstimator.hpp"

template<class TCostFunction, class TProximalOperator>
class ProximalCalculator
{
private:
    const LipschitzEstimator<TCostFunction> estimator;
    const TCostFunction costFunction;
    const TProximalOperator constraint;

    double linesearchGamma=0;
    Vector proximalStep;

    int maxNumberOfLinesearches = 10;

public:
    inline ProximalCalculator(
        TCostFunction costFunction,
        TProximalOperator constraint)
    {
        estimator = LipschitzEstimator<TCostFunction>(costFunction);
        costFunction = &costFunction;
        constraint = &constraint;
        proximalStep = Vector(costFunction.GetDimension());
        linesearchGamma = 0;
    }
    
    inline void Calculate(const Vector & location)
    {
        Vector gradient(location.size());
        const double cost = costFunction->Evaluate(location,gradient);
    
        if (linesearchGamma == 0) 
            linesearchGamma == estimator->Estimate(location, gradient);
    
        CalculateProximalStep(location, gradient);
        while (LinesearchCondition(location))
        {
            CalculateProximalStep(location,gradient);
        }
    }
    
    inline void CalculateProximalStep(
        const Vector & location,
        const Vector & gradient )
    {
        // x-df(x)*gamma
        (*proximalStep) = (*proximalStep) - linesearchGamma * gradient;

        //take prox to get proxg(x-df(x)*gamma)
        constraint->Evaluate((*proximalStep));
    }
    
    inline bool LinesearchCondition(const Vector & location)
    {
        return false; // TODO
    }
    
    inline Vector * GetProximalStep()
    {
        return proximalStep;
    }
    
    inline void GetResidual(
        const Vector & location, 
        Vector& residual)
    {
        //residual = ((location- (*proximalStep))/linesearchGamma);
    }
};
