#include "fbe.hpp"
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

constexpr int dimension_test_fbe = 2;
constexpr int degree_test_fbe = 2;
using vec_fbe = pnc::Vector<dimension_test_fbe, double>;

template <typename TLoc, typename TGrad>
using cost_function_fbe_test = pnc::test::poly<2, 2, 3>::type<TLoc, TGrad>;

using prox_op_fbe_test = pnc::BoxOp<double, vec_fbe>;
using p_calc_fbe_test = pnc::ProximalCalculator<cost_function_fbe_test, prox_op_fbe_test>;

TEST_CASE("Simple FBE example")
{
    const double low_fbe_test = -7;
    const double high_fbe_test = 7;
    prox_op_fbe_test prox_operator(low_fbe_test, high_fbe_test);
    p_calc_fbe_test prox_cal(prox_operator);

    vec_fbe startPosition = { 6, 10 };
    vec_fbe startGradient;
    auto startCost = cost_function_fbe_test<vec_fbe, vec_fbe>()(startPosition, startGradient);

    vec_fbe cache;
    auto start_location = pnc::LocationBuilder<
        vec_fbe,
        cost_function_fbe_test>::Build(
                std::move(startPosition),
                std::move(startGradient),
                startCost,
                cache);

    auto solution = pnc::Location<vec_fbe>( // will be filled in by prox_calcluator
        vec_fbe(),
        vec_fbe(),
        0,
        0);

    auto prox_step = prox_cal.Calculate(
            pnc::ProximalGradientStep(
                    start_location,
                    solution),
            p_calc_fbe_test::default_config);

    pnc::FBE<cost_function_fbe_test<vec_fbe, vec_fbe>, decltype(prox_operator)> fbe = {prox_operator};
     auto res = fbe
         .Eval(prox_step);

     REQUIRE(res == Approx(20.408).epsilon(0.1));
}
