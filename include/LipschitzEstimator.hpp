#pragma once

#include "ProximalGradientConfig.hpp"
#include <iostream>
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
    template <typename TVector, typename TCostFunction, typename TConfig>
    static double estimate(
        const TVector& location,
        const TVector& gradient,
        const TConfig& config,
        const TCostFunction cost_function)
    {
        // find delta= max{small number,10^{-6}*u_0}
        auto delta = (location*config.lipschitz_safetyValue)
            .cwiseMax(Vector::Ones(location.size()) * config.minimum_delta);

        auto newCost = cost_function(location + delta );

        return (newCost.second - gradient).norm() / delta.norm();
    }
};
}
