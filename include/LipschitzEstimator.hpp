#pragma once

#include"ProximalGradientConfig.hpp"
template<class TCostFunction>
class LipschitzEstimator
{
    private:
        const double delta_safety_value; 
        TCostFunction costFunction;
    public:
    
    inline LipschitzEstimator(
            TCostFunction costFunction, 
            double delta_safety_value): 
        costFunction(costFunction),
        delta_safety_value(delta_safety_value)
    {
    }
    
    //Estimate the lipschitz constant by using the numerical hessian as an estimation
    //  Theorem:
    //      ||gradient(x)|| < B
    //       f is B-lipschitz
    double estimate(const Vector& location,const Vector& gradient)
    {
        // find delta= max{small number,10^{-6}*u_0}
        double delta = location.cwiseMax(
            Vector::Ones(location.size())*delta_safety_value)
            .norm();
    
        Vector newLocation=location+(delta* Vector::Ones(location.size()));
        Vector newGradient;
    
        return (newGradient-gradient).norm()/delta;
    }
};

