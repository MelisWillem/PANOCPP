#include "VectorAlgebra.hpp"
#include "cmath"

namespace pnc {
namespace test {
    // Evaluates the value and the gradient of a polynomial of a certain
    // degree. Is provided by the user to the api
    template <
        auto degree,
        auto dimension,
        typename TInput,
        typename TOutput,
        auto weight = 1
             >
    struct Poly{
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

    template< auto degree, auto dimension, auto weight = 1>
    struct poly {
        template<typename TIn,typename TOut>
        using type = Poly<degree, dimension, TIn, TOut, weight>;
    };

    template<int degree, int dimension, int weight = 1>
    using poly_t = typename poly<degree, dimension, weight>::type;
}
}
