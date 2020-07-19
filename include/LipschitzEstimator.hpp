#pragma once

#include "VectorAlgebra.hpp"
#include <iostream>
#include "entities.hpp"
namespace pnc {

class LipschitzEstimator {
public:
    struct Config {
        const double lipschitz_safetyValue; // default in C# is: 1e-6
        const double minimum_delta;         // default in C# is: 1e-12
    };
    static constexpr auto default_config() -> Config
    {
        return  Config{
            1e-6,
            1e-12
        };
    }

    //Estimate the lipschitz constant by using the numerical hessian as an estimation
    //  Theorem:
    //      ||gradient(x)|| < B
    //       f is B-lipschitz
    template<
        typename TVector,
        typename TCostFunction,
        typename TConfig
            >
    static auto estimate(
        const Location<TVector>& position,
        const TConfig& config,
        const TCostFunction cost_function)
    {
        // find delta= max{small number,10^{-6}*u_0}
        auto delta = ComponentWiseMax(
                (config.lipschitz_safetyValue*position.location),
                pnc::VectorUnit<2,double>(config.minimum_delta));

        // atm we are allocating a new vector, each time we estimate.
        // TODO remove this so we reuse some memory, and don't allocate here

        auto newPosition = cost_function(position.location + delta);
        auto deltaNorm = delta*delta;

        return 0;
    }
};
}
