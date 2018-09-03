#include <Eigen/Core>

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> Matrix;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Vector;

#pragma once
class SolverLBFGS
{
private:
    int _bufferSize;
    int _cursor = 0;
    int _dimension;
    int _activeBufferSize=0;
    double _hessianEstimate = 0;

    Matrix* _s;
    Matrix* _y;
    Vector* _alpha;
    Vector* _rho;

    bool CheckIfValidUpdate(
        const Vector& gradientCurrentLocations,
        const Vector& potentialNewS,
        const Vector& potentialNewY);

public:
    SolverLBFGS(int bufferSize, int problemDimension);
    ~SolverLBFGS();

    void Solve(const Vector & gradient, Vector & outputDirection);
    bool UpdateHessian(
        const Vector& currentLocation,
        const Vector& currentGradient,
        const Vector& newLocation,
        const Vector& newGradient);
};

