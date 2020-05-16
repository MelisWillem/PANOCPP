#include "LipschitzEstimator.hpp"
#include "Poly.hpp"
#include "LipschitzEstimator.hpp"

TEST_CASE("LipschitzEstimatorTest")
{
    SECTION("SIMPLE_POLYGON_TEST")
    {
        const double lipschitzSafetyValue = 1e-6;
        const double minimumDelta = 1e-12;

        auto position = Vector(2);
        position << 6.0 , 10.0;
        std::pair<double,Vector> evaluation = pnc::test::poly2(position);
        auto testConfig = pnc::LipschitzEstimator::default_config();
        
        double result = pnc::LipschitzEstimator::estimate(position,evaluation.second,testConfig,pnc::test::poly2);
        const double expectedLipschitz = 2;
        REQUIRE(2 == Approx(result).epsilon(0.0001));
    }
}

