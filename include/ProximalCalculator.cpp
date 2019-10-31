#include "stdafx.h"
#include "ProximalCalculator.h"

ProximalCalculator::ProximalCalculator(
    IDerivableFunction costFunction,
    IProximalFunction constraint)
{
    _estimator = new LipschitzEstimator(costFunction);
    _costFunction = &costFunction;
    _constraint = &constraint;
    _proximalStep = new Vector(costFunction.GetDimension());
    _linesearchGamma = 0;
}

ProximalCalculator::~ProximalCalculator()
{
}

void ProximalCalculator::Calculate(const Vector & location)
{
    Vector gradient(location.size());
    const double cost = _costFunction->Evaluate(location,gradient);

    if (_linesearchGamma = 0) 
        _linesearchGamma = _estimator->Estimate(location, gradient);

    CalculateProximalStep(location, gradient);
    while (LinesearchCondition(location))
    {
        CalculateProximalStep(location,gradient);
    }
}

void ProximalCalculator::CalculateProximalStep(
    const Vector & location,
    const Vector & gradient )
{
    // x-df(x)*gamma
    (*_proximalStep) = (*_proximalStep) - _linesearchGamma * gradient;

    //take prox to get proxg(x-df(x)*gamma)
    _constraint->Evaluate((*_proximalStep));
}

bool ProximalCalculator::LinesearchCondition(const Vector & location)
{
    return false; // TODO
}

Vector * ProximalCalculator::GetProximalStep()
{
    return _proximalStep;
}

void ProximalCalculator::GetResidual(
    const Vector & location, 
    Vector& residual)
{
    //residual = ((location- (*_proximalStep))/_linesearchGamma);
}

