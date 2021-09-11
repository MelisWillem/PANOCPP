#include <panoc/LipschitzEstimator.hpp>
#include "Poly.hpp"
#include <panoc/entities.hpp>

TEST_CASE("LipschitzEstimatorTest")
{
    SECTION("SIMPLE_POLYGON_TEST")
    {
        using Vec = pnc::Vector<2, double>;
        pnc::test::Poly<2, 2> cost_function;
        auto position = Vec{6.0,10.0}; 
        auto gradient_start_position = Vec{};
        auto location_cost = cost_function(position,gradient_start_position);
        auto location = pnc::Location<Vec>(
                std::move(position),
                std::move(gradient_start_position),
                location_cost,
                0);// gamma is set to zero, as it doesn't matter to the estimator

        auto testConfig = pnc::LipschitzEstimator::default_config;

        auto cache = Vec();
        double result = pnc::LipschitzEstimator::estimate(
                location,
                testConfig,
				cost_function,
                cache);
        const double expectedLipschitz = 2;
        REQUIRE(expectedLipschitz == Approx(result).epsilon(0.0001));
    }
}
