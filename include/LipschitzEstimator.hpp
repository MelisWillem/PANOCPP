#pragma once

#include "VectorAlgebra.hpp"
#include <iostream>
#include "entities.hpp"
#include "math.h"

namespace pnc {

class LipschitzEstimator{
public:
    struct Config {
        // The lipschitz value is estimated over an delta of:
        // delta= max{MinimumDelta,LipschitzSafetyValue*u_0}
        
        const double lipschitz_safetyValue; // default in C# is: 1e-6
        const double minimum_delta;         // default in C# is: 1e-12
    };
    static constexpr Config default_config =
    {
        1e-6, // lipschitz_safetyValue
        1e-12 // minimum_delta
    };

    // Estimate the lipschitz constant by using the numerical hessian as an estimation
    //   Theorem:
    //      ||gradient(x)|| < B
    //       f is B-lipschitz
    // => cache: Memory location used to evaluate slightly deviated cost/gradient,
    //           -> Assume it's poluted after this call, and do NOT use the content.
    //           -> This avoids allocating an additional vector that is potentially large.
    template<
        typename TVector,
        typename TConfig,
        typename TCostFunction,
        typename data_type = typename TVector::data_type
            >
    static data_type estimate(
        const Location<TVector>& location,
        const TConfig& config,
        TCostFunction& cost_function,
        TVector& cache) 
    {
        // Find delta= max{small number,10^{-6}*u_0}
        auto delta = ComponentWiseMax(
                (config.lipschitz_safetyValue*location.location),
                pnc::VectorUnit<2,double>(config.minimum_delta));

        auto deviated_position = location.location + delta;
        auto& deviated_gradient = cache;
        cost_function(deviated_position, deviated_gradient);

        auto buff = location.gradient-deviated_gradient;
        return sqrt(buff * buff)/
            sqrt(delta*delta);
    }
};
}
