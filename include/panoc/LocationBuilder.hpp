#include<type_traits>
#include<utility>
#include<panoc/LipschitzEstimator.hpp>

namespace pnc{

    template<
        typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename data_type = typename TVec::data_type
        >
    struct LocationBuilder{

        template<typename TCostFunction>
        static Location<TVec> Build(
                TCostFunction& cost_function,
                TVec&& position,
                TVec&& gradient,
                data_type cost,
                TVec& cache)
        {
            using Estimator = LipschitzEstimator;
            auto config = Estimator::default_config;
            Location<TVec> location(
                    std::forward<TVecRef>(position),
                    std::forward<TVecRef>(gradient),
                    cost,
                    0);
            double safety_value_linesearch = 0.05;
            location.gamma = (1-safety_value_linesearch)/Estimator::estimate(
                    location,
                    config,
                    cost_function,
					cache);

            return location;
        }
    };
}
