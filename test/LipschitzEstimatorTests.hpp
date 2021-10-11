#include <panoc/LipschitzEstimator.hpp>
#include "Poly.hpp"
#include <panoc/entities.hpp>

TEST_CASE("LipschitzEstimatorTest")
{
    SECTION("SIMPLE_POLYGON_TEST")
    {
        using Vec = pnc::Vector<double>;
        constexpr auto degree = 2;
        pnc::test::Poly<degree> cost_function;
        Vec position = {6.0, 10.0}; 
        Vec gradient_start_position = {0, 0};
        auto location_cost = cost_function(position,gradient_start_position);
        auto location = pnc::Location<Vec>(
                std::move(position),
                std::move(gradient_start_position),
                location_cost,
                0);// gamma is set to zero, as it doesn't matter to the estimator

        const auto testConfig = pnc::LipschitzEstimator::default_config;

        Vec cache = { 0, 0 };
        const double result = pnc::LipschitzEstimator::estimate(
                location,
                testConfig,
				cost_function,
                cache);
        const double expectedLipschitz = 2;
        REQUIRE(expectedLipschitz == Approx(result).epsilon(0.0001));
    }
}
