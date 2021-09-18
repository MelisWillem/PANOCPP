#include <panoc/VectorAlgebra.hpp>
#include <cmath>

namespace pnc {
namespace test {
    // Evaluates the value and the gradient of a polynomial of a certain
    // degree. Is provided by the user to the api
    template <
        auto degree,
        auto weight = 1
             >
    struct Poly{
        template<
			typename TInput,
			typename TOutput>
        auto operator()(
                const TInput& position,
                TOutput& output_gradient)
        {
            double cost = 0;
            for (auto i = 0; i < position.size(); i++) {
                output_gradient[i] = weight*(degree)*pow(position[i], (degree - 1));
                cost += weight*pow(position[i], degree);
            }
            return cost;
        }
    };
}
}
