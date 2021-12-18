#include <panoc/fbe.hpp>
// var prox = new ProxBox(size: 7, penalty: 1, dimension: 2);
//
// Manual calculation of this fbe:
// f(x) = 3x^2
// df(x) = 6x
// gx(x) = box{7}
//
// -----
// start position: [6;10]
// cost at this position = 36*3+100*3=408
// df at this position = [6*6;6*10] = [36;60]

// prox step taken to
// -----
// position: [0.299999;0.499999]
// cost at this position: 1.019999
// df at this position: [1.79999;2.99999]
// gamma: 0.15833
// constraint cost: 0

// some intermediate values
// direction = x_old - x_new = [6;10] -[0.299999;0.499999] = [5.70000;9.50000]
// direction* direction = 122.74000
// df'*direction = 775.20000

// so the fbe becomes:
// fbe = fx + gx_new - df'*direction + (1/(gamma*2))*(direction'*direction)
//     = (408 + 0 - 775.20000 + (1/(0.15833*2))*(122.74000))
//     = 20.408


TEST_CASE("Simple FBE example")
{
	using vec_fbe = pnc::Vector<double>;

    const double low_fbe_test = -7;
    const double high_fbe_test = 7;
    pnc::BoxOp<double> prox_operator(low_fbe_test, high_fbe_test);
    constexpr auto degree = 2;
    constexpr auto weight = 3;
    constexpr int dimension = 2;
    pnc::test::Poly<degree, weight> cost_function;
	pnc::ProximalCalculator prox_cal{cost_function, prox_operator};

    vec_fbe startPosition = { 6, 10 };
    vec_fbe startGradient (dimension);
    auto startCost = cost_function(startPosition, startGradient);

    vec_fbe cache(dimension);
    auto start_location = pnc::LocationBuilder<
        vec_fbe>::Build(
				cost_function,
                std::move(startPosition),
                std::move(startGradient),
                startCost,
                cache);

    auto solution = pnc::Location<vec_fbe>( // will be filled in by prox_calcluator
        vec_fbe(dimension),
        vec_fbe(dimension),
        0,
        0);

    prox_cal.Calculate<vec_fbe>(
                    start_location,
                    solution);

    pnc::FBE fbe = {cost_function, prox_operator};
     auto res = fbe
         .Eval(start_location, solution);

     REQUIRE(res == Approx(20.408).epsilon(0.1));
}
