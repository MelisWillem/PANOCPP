#include<type_traits>
#include<utility>
#include"LipschitzEstimator.hpp"

namespace pnc{

    template<
        typename TVecRef,
        template<typename, typename> typename TCostFunction,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename data_type = typename TVec::data_type
        >
    struct LocationBuilder{
        static Location<TVec> Build(
                TVec&& position,
                TVec&& gradient,
                data_type cost,
                TVec& cache)
        {
            using Estimator = LipschitzEstimator<TCostFunction>;
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
                    cache);

            return location;
        }
    };
}
