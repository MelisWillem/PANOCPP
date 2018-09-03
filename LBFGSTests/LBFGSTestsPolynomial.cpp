#include "stdafx.h"
#include "CppUnitTest.h"
#include "../LBFGS/SolverLBFGS.h"
#include <Math.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LBFGSTests
{		
	TEST_CLASS(LBFGSTestPolynomial)
	{
    private:
        double Polynomial2thDegree(Vector& location) 
        {
            return location.dot(location);
        }
        void Polynomial2thDegreeGradient(Vector& location, Vector& gradient)
        {
            gradient =  2*location;
        }
	public:
		
		TEST_METHOD(GIVEN_POLYNOMIAL_2TH_DEGREE_CHECK_SOLVER)
		{
            const unsigned int dimension = 2;
            const unsigned int bufferSize = 5;
            SolverLBFGS* solver = new SolverLBFGS(bufferSize, dimension);

            Vector position(2);
            position << 0.5, 0.5; // start at location (5;5)
            Vector newPosition(2);

            Vector gradientPosition(2);
            Polynomial2thDegreeGradient(position, gradientPosition);
            Vector newGradient(2);

            Vector direction(2);

            for (unsigned int i = 0; i < 2; i++)
            {
                solver->Solve(gradientPosition, direction);

                newPosition = position + direction;
                Polynomial2thDegreeGradient(newPosition, newGradient);

                solver->UpdateHessian(position, gradientPosition, newPosition, newGradient);

                position = newPosition;
                gradientPosition = newGradient;
            }
            
            Assert::AreEqual(0, position(0), 0.01);
            Assert::AreEqual(0, position(1), 0.01);
		}

	};
}