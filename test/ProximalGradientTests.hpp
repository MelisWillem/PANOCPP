#include<panoc/ProximalCalculator.hpp>
#include<panoc/proxOperators.hpp>
#include<panoc/LocationBuilder.hpp>

using CostFunction = pnc::test::Poly<5, 2>;

TEST_CASE("Polygon test")
{
    SECTION("2th degree polygon")
    {
        const int number_of_iterations = 100;
        const int dimension = 2;

        using Vec = pnc::Vector<dimension,double>;
        using prox = pnc::NormBox<double>;

        auto initialPosition = Vec{0.5,0.5};
        auto initialGradient = Vec{};
        CostFunction cost_function;
        auto initialCost = cost_function(initialPosition,initialGradient);
        
        auto solution = pnc::Location<Vec>(
            Vec(),
            Vec(),
            0,
            0);
        auto initial_location = pnc::LocationBuilder<Vec>::Build(
            cost_function,
            std::move(initialPosition),
            std::move(initialGradient),
            initialCost,
            solution.location);

        auto offset = double{2};
        prox prox_op = { offset };
        auto calc = pnc::ProximalCalculator<CostFunction,prox>(cost_function,prox_op );
        for(int i=0;i<number_of_iterations;i++)
        {
            calc.Calculate(
                    initial_location,
                    solution,
                decltype(calc)::default_config);
            initial_location.location = solution.location;
            initial_location.gradient = solution.gradient;
            initial_location.gamma = solution.gamma;
            initial_location.cost = solution.cost;
        }

        double expected = 0.118816;
        REQUIRE( std::abs(expected - initial_location.location[0]) < 1e-6);
        REQUIRE( std::abs(expected - initial_location.location[1]) < 1e-6);
    }
}

