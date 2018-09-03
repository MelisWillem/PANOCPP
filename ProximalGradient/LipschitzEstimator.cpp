#include "stdafx.h"
#include "LipschitzEstimator.h"
#include "IDerivableFunction.h"


LipschitzEstimator::LipschitzEstimator(IDerivableFunction & costFunction)
{
    _costFunction = &costFunction;
}


LipschitzEstimator::~LipschitzEstimator()
{
}


//Estimate the lipschitz constant by using the numerical hessian as an estimation
//  Theorem:
//      ||gradient(x)|| < B
//       f is B-lipschitz
double LipschitzEstimator::Estimate(Vector& location,Vector& gradient)
{
    // find delta= max{small number,10^{-6}*u_0}
    double delta = location.cwiseMax(
        Vector::Ones(location.size())*_delta_safety_value)
        .norm();

    Vector newLocation=location+(delta* Vector::Ones(location.size()));
    Vector newGradient;

    return (newGradient-gradient).norm()/delta;
}
