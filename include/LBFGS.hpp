#include <memory>
#include <vector>

inline int getFloatingIndex(int i,int cursor,int bufferSize) {
    return (cursor - 1 - i + bufferSize) % bufferSize;
}

class LBFGS
{
private:
    int _bufferSize;
    int _dimension;
    int _cursor = 0;
    int _activeBufferSize=0;
    double _hessianEstimate = 0;

    std::unique_ptr<Matrix> _s;
    std::unique_ptr<Matrix> _y;
    std::unique_ptr<Vector> _alpha;
    std::unique_ptr<Vector> _rho;
public:
    inline LBFGS(int bufferSize, int problemDimension)
    {
        _s = std::make_unique<Matrix>(problemDimension, bufferSize); // one element extra used in update
        _y = std::make_unique<Matrix>(problemDimension, bufferSize); // one element extra used in update
    
        _bufferSize = bufferSize;
        _dimension = problemDimension;
    
        _activeBufferSize = 0;
    }
    
    inline void solve(
        const Vector& gradient,
        Vector& outputDirection)
    {
        if (_activeBufferSize == 0) 
        {
            outputDirection = -gradient;
            return;
        }

        outputDirection = gradient;
    
        Vector alpha(_activeBufferSize);
        Vector rho(_activeBufferSize);
        for (int absolute_i = 0; absolute_i < _activeBufferSize; ++absolute_i)
        {
            unsigned int i = getFloatingIndex(absolute_i, _cursor, _bufferSize);
    
            rho(i) = 1/(_s->col(i).dot(_y->col(i)));
            alpha(i) = rho(i)*(_s->col(i).dot(outputDirection));
    
            outputDirection = outputDirection -(alpha(i)*_y->col(i));
        }
        
        outputDirection = outputDirection * _hessianEstimate;
    
        double beta;
        for (int absolute_i = _activeBufferSize-1; absolute_i > -1; --absolute_i)
        {
            unsigned int i = getFloatingIndex(absolute_i, _cursor, _bufferSize);
    
            beta = rho(i)*(_y->col(i).dot(outputDirection));
            outputDirection = outputDirection + (alpha(i)-beta)*_s->col(i);
        }
    
        outputDirection = -outputDirection;
    }
    
    inline bool CheckIfValidUpdate(
        const Vector& gradientCurrentLocations,
        const Vector& potentialS,
        const Vector& potentialY)
    {
        // Theoretical condition:
        // update if (y^Ts)/||s || ^ 2 > epsilon * || grad(x) ||
        double value = potentialS.dot(potentialY)
            / potentialS.dot(potentialS);
    
        return value > gradientCurrentLocations.norm()*(1e-12);
    }
    
    inline bool updateHessian(
        const Vector& currentLocation, 
        const Vector& currentGradient, 
        const Vector& newLocation, 
        const Vector& newGradient)
    {
        Vector potentialS = newLocation - currentLocation;
        Vector potentialY = newGradient - currentGradient;
    
        if (this->CheckIfValidUpdate(currentGradient, potentialS, potentialY))
        {
            _s->col(_cursor) = potentialS;
            _y->col(_cursor) = potentialY;
    
            _hessianEstimate = (potentialS.dot(potentialS))
                / (potentialS.dot(potentialS));
    
            if (_activeBufferSize < _bufferSize)
            {
                _activeBufferSize++;
            }
            _cursor = (_cursor + 1) % _bufferSize;
            return true;
        }
        return false;
    }
};
