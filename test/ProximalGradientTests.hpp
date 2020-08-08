#include"ProximalCalculator.h" 

#define CATCH_CONFIG_MAIN
#include"catch.hpp"

TEST_CASE("Polygon test")
{
    SECTION("2th degree polygon")
    {
        int number_of_iterations = 99;
        int degree

        auto costFunction = pnc::test::poly<2,2>;
        // The proximal operator == Normbox -> still todo

        auto initial_position = pnc::Vector{0.5,0.5};
        auto location = costFunction(initial_position);

        auto testConfig = pnc::LipschitzEstimator::default_config;

        //auto vec = pnc::Vector<1,int>(2);
        ///REQUIRE(pnc::Box<int,1,3>(vec)[0] == 2);
    }
}

