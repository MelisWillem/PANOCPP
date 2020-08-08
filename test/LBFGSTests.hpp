#include <math.h>
#include<LBFGS.hpp>

void RosenBrockGradient(pnc::Vector<2,double>& location, pnc::Vector<2,double>& gradient)
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
        using Vector = pnc::Vector<2,double>;
        static constexpr unsigned int dimension = 2;
        static constexpr unsigned int bufferSize = 20;
        auto solver = pnc::LBFGS<bufferSize,double,dimension> ();

        auto position = Vector(-1.2,1.0);// start at location (-1.2;1.)
        auto newPosition = Vector();

        auto gradientPosition = Vector();
        RosenBrockGradient(position, gradientPosition);
        auto newGradient = Vector();

        auto direction = Vector();

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
