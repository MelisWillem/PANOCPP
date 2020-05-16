#include "EigenAlgebra.hpp"
#include "cmath"

namespace pnc {
namespace test {
    // Evaluates the value and the gradient of a polynomial of a certain
    // degree.
    template <int degree>
    std::pair<double, Vector> poly(const Vector& position)
    {
        // return const + gradient
        // do we require a move of a vector to put this result in?
        double cost =0;
        auto gradient = position;
        for(auto i=0;i<gradient.size();i++)
        {
            gradient[i] = (degree)*pow(gradient[i],(degree-1));
            cost += pow(position[i],degree);
        }
        return std::make_pair(cost, gradient);
    }

    constexpr auto poly2 = poly<2>;
}
}
