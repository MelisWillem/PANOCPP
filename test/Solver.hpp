#include "panoc.hpp"

TEST_CASE("Given_poly_of_6th_degree_solve")
{
	using namespace pnc::test;
	using namespace pnc;

	int number_of_iterations = 5;
	constexpr int degree = 6;
	constexpr int dimension = 2;

	Poly<degree, dimension> cost_function;
	double offset = 2;
	NormBox prox_op = { offset };

	PanocConfig config = {
		5, // max_iterations
		10, // max_fbe_iterations
		1e-15 // min_residual
	};
	Panoc solver = { cost_function, prox_op, config };

	Vector<2> solution = { 0.5,0.5 }; 
	solver.Solve(solution);

	REQUIRE(solution[0] == Approx(0.22128222).epsilon(1e-6));
	REQUIRE(solution[1] == Approx(0.22128222).epsilon(1e-6));
}

TEST_CASE("Given_poly_of_6th_degree_solve_2th_test")
{
	using namespace pnc::test;
	using namespace pnc;

	int number_of_iterations = 5;
	constexpr int degree = 6;
	constexpr int dimension = 2;

	Poly<degree, dimension> cost_function;
	double offset = 2;
	NormBox prox_op = { offset };

	PanocConfig config = {
		10, // max_iterations
		10, // max_fbe_iterations
		1e-15 // min_residual
	};
	Panoc solver = { cost_function, prox_op, config };

	Vector<2> solution = { 1,1 }; 
	solver.Solve(solution);

	REQUIRE(solution[0] == Approx(0.204269).epsilon(1e-5));
	REQUIRE(solution[1] == Approx(0.204269).epsilon(1e-5));
}



