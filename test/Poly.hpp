#include "VectorAlgebra.hpp"
#include "cmath"

namespace pnc {
namespace test {
    // Evaluates the value and the gradient of a polynomial of a certain
    // degree. Is provided by the user to the api
    template <
        int degree,
        int dimension,
        typename TInput,
        typename TOutput
             >
    struct Poly{
        auto operator()(
                const TInput& position,
                TOutput& gradient)
        {
            using Vec = Vector<dimension, double>;

            double cost = 0;
            for (auto i = 0; i < Vec::size; i++) {
                gradient[i] = (degree)*pow(position[i], (degree - 1));
                cost += pow(position[i], degree);
            }
            return cost;
        }
    };

    template< int degree, int dimension>
    struct poly {
        template<typename TIn,typename TOut>
        using type = Poly<degree,dimension,TIn,TOut>;
    };
}
}
