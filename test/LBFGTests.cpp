#include <Math.h>


namespace LBFGSTests
{
    TEST_CLASS(LBFGSTestRosen)
    {
    private:
        void RosenBrockGradient(Vector& location, Vector& gradient)
        {
            double a = 1;
            double b = 100;

            /* Matlab: df = @(x) [-2*(a-(b+1)*x(1)+b*x(2)); 2*b*(x(2)-x(1)) ]; */
            gradient(0) = -2 * (a - (b + 1)*location(0) + b * location(1));
            gradient(1) = 2 * b*(location(1) - location(0));
        }
    public:

        TEST_METHOD(GIVEN_ROSENBROCK_CHECK_SOLVER)
        {
            const unsigned int dimension = 2;
            const unsigned int bufferSize = 20;
            SolverLBFGS* solver = new SolverLBFGS(bufferSize, dimension);

            Vector position(2);
            position << -1.2, 1.; // start at location (-1.2;1.)
            Vector newPosition(2);

            Vector gradientPosition(2);
            RosenBrockGradient(position, gradientPosition);
            Vector newGradient(2);

            Vector direction(2);

            for (unsigned int i = 0; i < 4; i++)
            {
                solver->Solve(gradientPosition, direction);

                newPosition = position + direction;
                RosenBrockGradient(newPosition, newGradient);

                solver->UpdateHessian(position, gradientPosition, newPosition, newGradient);

                position = newPosition;
                gradientPosition = newGradient;
            }

            Assert::AreEqual(1, position(0), 0.01);
            Assert::AreEqual(1, position(1), 0.01);
        }

        TEST_METHOD(GIVEN_ROSENBROCK_CHECK_SOLVER_WHEN_OVER_ITERATING)
        {
            const unsigned int dimension = 2;
            const unsigned int bufferSize = 20;
            SolverLBFGS* solver = new SolverLBFGS(bufferSize, dimension);

            Vector position(2);
            position << -1.2, 1.; // start at location (-1.2;1.)
            Vector newPosition(2);

            Vector gradientPosition(2);
            RosenBrockGradient(position, gradientPosition);
            Vector newGradient(2);

            Vector direction(2);

            for (unsigned int i = 0; i < 100; i++)
            {
                solver->Solve(gradientPosition, direction);

                newPosition = position + direction;
                RosenBrockGradient(newPosition, newGradient);

                solver->UpdateHessian(position, gradientPosition, newPosition, newGradient);

                position = newPosition;
                gradientPosition = newGradient;
            }

            Assert::AreEqual(1, position(0), 1e-12);
            Assert::AreEqual(1, position(1), 1e-12);
        }
    };
}
