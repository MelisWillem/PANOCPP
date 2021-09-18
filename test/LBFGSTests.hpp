#include <math.h>
#include <panoc/LBFGS.hpp>

void RosenBrockGradient(pnc::Vector<double>& location, pnc::Vector<double>& gradient)
{
    double a = 1;
    double b = 100;

    /* Matlab: df = @(x) [-2*(a-(b+1)*x(1)+b*x(2)); 2*b*(x(2)-x(1)) ]; */
    gradient[0] = -2 * (a - (b + 1)*location[0] + b * location[1]);
    gradient[1] = 2 * b*(location[1] - location[0]);
}

TEST_CASE("LBFGSTestRosen")
{
    SECTION("GIVEN_ROSENBROCK_CHECK_SOLVER")
    {
        using Vector = pnc::Vector<double>;
        static constexpr unsigned int dimension = 2;
        static constexpr unsigned int bufferSize = 20;
        auto solver = pnc::LBFGS<
            bufferSize,
            typename Vector::data_type,
            typename Vector::size_type> (dimension);

        auto position = Vector{-1.2,1.0};// start at location (-1.2;1.)
        auto newPosition = Vector(position.size());

        auto gradientPosition = Vector(position.size());
        RosenBrockGradient(position, gradientPosition);
        auto newGradient = Vector(position.size());

        Vector direction (position.size());

        for (unsigned int i = 0; i < 4; i++)
        {
            solver.solve(gradientPosition, direction);

            newPosition = position + direction;
            RosenBrockGradient(newPosition, newGradient);

            solver.updateHessian(position, gradientPosition, newPosition, newGradient);

            position = newPosition;
            gradientPosition = newGradient;
        }

        REQUIRE(1 == Approx(position[0]).epsilon(1e-12));
        REQUIRE(1 == Approx(position[1]).epsilon(1e-12));
    }
};
