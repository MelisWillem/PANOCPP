#include "LipschitzEstimator.hpp"
#include "Poly.hpp"


TEST_CASE("LipschitzEstimatorTest")
{
    SECTION("SIMPLE_POLYGON_TEST")
    {
        const double lipschitzSafetyValue = 1e-6;
        const double minimumDelta = 1e-12;

        //position << 6.0, 10.0;
        double* positionData = (double*)malloc(sizeof(double) * 2);
        auto position = pnc::Vector<2, double>(positionData); // Todo use a variadic template on helper function to create vector's  with a certain constants.
        position[0] = 6.0;
        position[1] = 10.0;

        auto costFunction = pnc::test::poly<2,2>;

        auto evaluation = costFunction(position);
        auto testConfig = pnc::LipschitzEstimator::default_config();

        double result = pnc::LipschitzEstimator::estimate(position, evaluation.second, testConfig, costFunction);
        const double expectedLipschitz = 2;
        REQUIRE(2 == Approx(result).epsilon(0.0001));
    }
}
