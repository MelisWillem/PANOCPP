#pragma once

#include <vector>
#include "entities.hpp"
#include "LipschitzEstimator.hpp"

class ProximalGradientStep
{
public:

    struct Config{
            /// The linesearch condition should be :
            // fNew > f - df.DotProduct(df) 
            //     + (1 ) / (2 * newLocation.Gamma) * (direction.DotProduct(direction))
            //
            // In reality it is:
            // fNew > f - df.DotProduct(df)
            //     + (1 - safetyValueLineSearch) / (2 * newLocation.Gamma) * directionSquaredNorm
            //     + 1e-6 * f;
            const double safety_value_line_search;
            // The linesearch parameter gamma can never be smaller then this
            const double min_gamma_value;
    };
    static constexpr Config default_config =
    {
        0.05, // safety_value_line_search
        1e-15 // min_gamma_value
    }

    template<
        typename TVector,
        typename TCostFunction, 
        typename TProximalOperator,
        typename data_type = typename TVector::data_type
        >
    static constexpr  ProxLocation<TVector> Calculate(
            const PncLocation<TVector>& current,
            ProxLocation<TVector>&& newLocatoin, // this is returned
            TCostFunction func,
            TProximalOperator prox,
            Config config)
    {
        // Estimate gamma if their is no estimation from a previous run.
        data_type gamma = std::visit(overloaded{
                [](Location<TVector> loc) {
                return LipschitzEstimator
                    .estimate(loc,LipschitzEstimator::default_config,func);
                },
                [](ProxLocation<TVector> proxLoc) { return proxLoc.gamma; },
                },current);

        newLocation = TakeProxStep(location,newLocation,func,prox,gamma);
        while (LinesearchCondition(location,newLocation)
                && (gamma/2 > config.min)
        {
            gamma = gamma/2;
            newLocation = TakeProxStep(location,newLocation,func,prox,gamma);
        }

        return newLocation;
    }
    
    template<
        typename TVector,
        typename data_type = typename TVector::data_type,
        typename TCostFunction, 
        typename TProximalOperator,
        >
    static constexpr ProxLocation TakeProxStep(
        const Location<TVector>& current,
        ProxLocation<TVector>&& newLocation,
        TCostFunction func,
        TProximalOperator prox,
        data_type gamma)
    {
        // Todo : get this to work on lipschitz estimator first
        // Todo : implement proxbox to see if this works
        func.evaluate(prox(current.location - gamma*current.gradient),newLocation);
        return newLocation;
    }
    
    template<
        typename TVector,
        typename data_type = typename TVector::data_type
        >
    static constexpr bool LinesearchCondition(
            const Location<TVector>& current,
            const ProxLocation<TVector>& candidate,
            const data_type safety_value_line_search)
    {
        auto direction_squared_norm = (current.location - candidate.location)
            * (current.location - candidate.location);

        auto f = current.cost;
        auto df = current.gradient;
        auto f_new = candidate.cost;
        auto df_new = candidate.gradient;

        return fNew > f - df*df
                + (1 - safety_value_line_search) / (2 * candidate.Gamma) 
                    * direction_squared_norm
                + 1e-6 * f;
    }
};
