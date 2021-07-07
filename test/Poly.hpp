#include <VectorAlgebra.hpp>
#include <cmath>

namespace pnc {
namespace test {
    // Evaluates the value and the gradient of a polynomial of a certain
    // degree. Is provided by the user to the api
    template <
        auto degree,
        auto dimension,
        auto weight = 1
             >
    struct Poly{
        template<
			typename TInput,
			typename TOutput>
        auto operator()(
                const TInput& position,
                TOutput& gradient)
        {
            using Vec = Vector<dimension, double>;

            double cost = 0;
            for (auto i = 0; i < Vec::size; i++) {
                gradient[i] = weight*(degree)*pow(position[i], (degree - 1));
                cost += weight*pow(position[i], degree);
            }
            return cost;
        }
    };
}
}
