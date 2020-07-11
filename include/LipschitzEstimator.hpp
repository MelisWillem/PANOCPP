#pragma once

#include "VectorAlgebra.hpp"
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
    template <typename TVectorLocation,typename TVectorGradient, typename TCostFunction, typename TConfig>
    static double estimate(
        const TVectorLocation& location,
        const TVectorGradient& gradient,
        const TConfig& config,
        const TCostFunction cost_function)
    {
        // find delta= max{small number,10^{-6}*u_0}
        //auto delta = (location*config.lipschitz_safetyValue)
        //    .cwiseMax(Vector::Ones(location.size()) * config.minimum_delta);
        // These 2 ungly constructs can be solved by introducing a include_if constraint on the operators between vector-vector such that vector-data_type operations ca be defined
        //auto left = location*VectorUnit<TVectorLocation::size,TVectorLocation::data_type>(config.lipschitz_safetyValue);
        //auto right = pnc::VectorUnit<TVectorLocation::size,TVectorLocation::data_type>(config.minimum_delta);// TODO: remove the manual double override
        //auto delta = pnc::ComponentWiseMax(left,right);

        //auto newCost = cost_function((location + delta )|ToVector());

        //return Norm(newCost.second - gradient) / Norm(delta);
        return 0;
    }
};
}
