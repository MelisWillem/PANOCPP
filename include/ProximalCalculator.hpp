#pragma once

#include <vector>
#include "entities.hpp"
#include "LipschitzEstimator.hpp"
#include "vectorAlgebra/tostring.hpp"
#include "VectorAlgebra.hpp"
#include <iostream>

namespace pnc{

// Contains a proximal gradient step:
// -. Does not own the vector's itself !
template<typename TVec>
struct ProximalGradientStep{
    ProximalGradientStep(
            const Location<TVec>& c, 
            Location<TVec>& p) :
        current(c),
        proximal(p) {}

    const Location<TVec>& current;
    Location<TVec>& proximal;

};

template< 
    typename TCostFunction,
    typename TProximalOperator
    >
class ProximalCalculator{
public:
    using Estimator = LipschitzEstimator;
    TProximalOperator& prox_;

    struct Config{
            /// The linesearch condition should be :
            // fNew > f - df.DotProduct(df) 
            //     + (1 ) / (2 * new_location.Gamma) * (direction.DotProduct(direction))
            //
            // In reality it is:
            // fNew > f - df.DotProduct(df)
            //     + (1 - safetyValueLineSearch) / (2 * new_location.Gamma) * directionSquaredNorm
            //     + 1e-6 * f;
            const double safety_value_line_search;
            // The linesearch parameter gamma can never be smaller then this
            const double min_gamma_value;
    };
    static constexpr Config default_config =
    {
        0.05, // safety_value_line_search
        10 // min_gamma_value
    };

    TCostFunction& cost_function_;

    ProximalCalculator(TCostFunction& cost_function, TProximalOperator& prox):
        cost_function_(cost_function),
        prox_(prox) {}

    template<
        typename TVector,
        typename data_type = typename TVector::data_type
        >
    ProximalGradientStep<TVector> Calculate(
            ProximalGradientStep<TVector>&& step,
            const Config config)  
    {
        TakeProxStep( 
            step.current,
            step.current.gamma, 
            step.proximal); 

        while (!LinesearchCondition(step,config.safety_value_line_search) && 
                (step.proximal.gamma/data_type{2} > config.min_gamma_value))
        {
            TakeProxStep(
                step.current,
                step.proximal.gamma/data_type{2},
                step.proximal);
        }

        return step;
    }
    
private:
    // use the value of current_location to calculate a
    // new proximal gradient step.
    template<
        typename TVector,
        typename data_type = typename TVector::data_type
        >
    void TakeProxStep(
        const Location<TVector>& current,
        const data_type gamma,
        Location<TVector>& proximal) const
    {
        proximal.gamma = gamma;
        proximal.location=prox_(current.location - gamma*current.gradient);
        proximal.cost = cost_function_( 
            proximal.location,
            proximal.gradient);
    }
    
    template<
        typename TVector,
        typename data_type = typename TVector::data_type
        >
    bool LinesearchCondition(
            const ProximalGradientStep<TVector>& step,
            const data_type safety_value_line_search)  const
    {
        const auto direction_squared_norm = (step.current.location - step.proximal.location)
            * (step.current.location - step.proximal.location);

        const auto f = step.current.cost;
        const auto& df = step.current.gradient;
        const auto f_new = step.proximal.cost;
        //const auto& df_new = step.proximal.gradient;

        return f_new > f - df*df
                + (1 - safety_value_line_search) / (2 * step.proximal.gamma) 
                    * direction_squared_norm
                + 1e-6 * f;
    }
};

}
