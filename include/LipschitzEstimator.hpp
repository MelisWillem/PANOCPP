#pragma once

#include "VectorAlgebra.hpp"
#include <iostream>
#include "entities.hpp"
namespace pnc {

class LipschitzEstimator {
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
    template<
        typename TVector,
        typename data_type = typename TVector::data_type,
        typename TCostFunction,
        typename TConfig
            >
    static data_type estimate(
        const Location<TVector>& location,
        const TConfig& config,
        const TCostFunction cost_function)
    {
        // Find delta= max{small number,10^{-6}*u_0}
        auto delta = ComponentWiseMax(
                (config.lipschitz_safetyValue*location.location),
                pnc::VectorUnit<2,double>(config.minimum_delta));

        // This vector could be moved in, and moved back out, something to look into
        auto newLocation = Vector<TVector::size,data_type>() ;
        newLocation = location.location + delta;
        auto newPosition = cost_function(newLocation);

        return (newPosition.location*newPosition.location)/(delta*delta);
    }
};
}
