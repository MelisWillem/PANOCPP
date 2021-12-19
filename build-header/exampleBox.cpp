#include "panocpplib.hpp"

int main(){
	constexpr int number_of_iterations = 5;
	constexpr int degree = 6;
	constexpr int dimension = 2;

	auto cost_function = [degree](const auto& input, auto& output)
	{
		const auto cost = pow(input[0], degree) + pow(input[1], degree);
		output[0] = degree*pow(input[0], degree-1);
		output[1] = degree*pow(input[1], degree-1);
		
		return cost;
	};
	double offset = 2;
	pnc::BoxOp<double> box{ -4.0,4.0 };

	// Make a panoc solver the uses double
	// for value in vector, 
	// and int for indices in vector.
	pnc::PanocConfig<double, int> config = {
		10, // max_iterations
		10, // max_fbe_iterations
		1e-15, // min_residual
		5 // lbfgs cache size
	};
	pnc::Panoc solver = { cost_function, box, config, dimension};

	pnc::Vector<double> solution = { 1,1 }; 
	solver.Solve(solution);

	std::cout << "x[0]=" << solution[0]; // 0.204269
    std::cout << " ";
	std::cout << "x[1]=" << solution[1]; // 0.204269

	return EXIT_SUCCESS;
}
