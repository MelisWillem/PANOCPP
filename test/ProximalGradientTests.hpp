#include"ProximalCalculator.hpp" 
#include"proxOperators.hpp"
#include"LocationBuilder.hpp"

template<typename TIn,typename TOut>
using CostFunction = pnc::test::poly<5,2>::type<TIn,TOut>;

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
        auto initialCost = CostFunction<decltype(initialPosition),decltype(initialGradient)>()
            (initialPosition,initialGradient);
        
        auto solution = pnc::Location<Vec>(
            Vec(),
            Vec(),
            0,
            0);
        auto initial_location = pnc::LocationBuilder<Vec,CostFunction>::Build(
            std::move(initialPosition),
            std::move(initialGradient),
            initialCost,
            solution.location);

        auto offset = double{2};
        auto calc = pnc::ProximalCalculator<CostFunction,prox>(prox(offset));
        for(int i=0;i<number_of_iterations;i++)
        {
            auto step = calc.Calculate(
                pnc::ProximalGradientStep(
                    initial_location,
                    solution),
                decltype(calc)::default_config);
            initial_location.location = step.proximal.location;
            initial_location.gradient = step.proximal.gradient;
            initial_location.gamma = step.proximal.gamma;
            initial_location.cost = step.proximal.cost;
        }

        double expected = 0.118816;
        REQUIRE( std::abs(expected - initial_location.location[0]) < 1e-6);
        REQUIRE( std::abs(expected - initial_location.location[1]) < 1e-6);
    }
}

