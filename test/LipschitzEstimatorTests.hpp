#include "LipschitzEstimator.hpp"
#include "Poly.hpp"
#include "entities.hpp"

template<typename TIn,typename TOut>
using costFunction = pnc::test::poly<2,2>::type<TIn,TOut>;

TEST_CASE("LipschitzEstimatorTest")
{
    SECTION("SIMPLE_POLYGON_TEST")
    {
        using Vec = pnc::Vector<2, double>;

        auto position = Vec{6.0,10.0}; 
        auto gradient_start_position = Vec{};
        auto location_cost = costFunction<Vec,Vec>()(position,gradient_start_position);
        auto location = pnc::Location<Vec>(
                std::move(position),
                std::move(gradient_start_position),
                location_cost);

        auto testConfig = pnc::LipschitzEstimator<costFunction>::default_config;

        auto cache = Vec();
        double result = pnc::LipschitzEstimator<costFunction>::estimate(
                location,
                testConfig,
                cache);
        const double expectedLipschitz = 2;
        REQUIRE(expectedLipschitz == Approx(result).epsilon(0.0001));
    }
}
