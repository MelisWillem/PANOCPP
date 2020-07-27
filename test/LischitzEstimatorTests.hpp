#include "LipschitzEstimator.hpp"
#include "Poly.hpp"


TEST_CASE("LipschitzEstimatorTest")
{
    SECTION("SIMPLE_POLYGON_TEST")
    {
        auto position = pnc::Vector<2, double>{6.0,10.0}; 

        auto costFunction = pnc::test::poly<2,2>;

        auto location = costFunction(position);
        auto testConfig = pnc::LipschitzEstimator::default_config;

        double result = pnc::LipschitzEstimator::estimate(location, testConfig, costFunction);
        const double expectedLipschitz = 2;
        REQUIRE(expectedLipschitz == Approx(result).epsilon(0.0001));
    }
}
