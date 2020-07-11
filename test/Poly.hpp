#include "VectorAlgebra.hpp"
#include "cmath"

namespace pnc {
namespace test {
    // Evaluates the value and the gradient of a polynomial of a certain
    // degree.
    template <int degree, int dimension>
    //std::pair<double,Vector<dimension,double>>
    auto poly(const Vector<dimension, double>& position)
    {
        using Vec = Vector<dimension, double>;
        // return const + gradient
        // do we require a move of a vector to put this result in?
        double cost = 0;
        Vector<dimension, double> gradient((double*)malloc(sizeof(double) * dimension)); // Start from copy.
        for (auto i = 0; i < Vec::size; i++) {
            gradient[i] = (degree)*pow(position[i], (degree - 1));
            cost += pow(position[i], degree);
        }
        return std::make_pair(cost, std::move(gradient));
        //return std::make_pair(cost, (gradient));// this fails, for some reason he doesn't want to autmatically move
    }

}
}
