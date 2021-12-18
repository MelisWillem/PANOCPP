#pragma once

#include<math.h>
#include<vector>
#include<panoc/VectorAlgebra.hpp>

namespace pnc{


template<
    typename data_type,
    typename size_type
        >
class LBFGS
{
private:
    using Vec = Vector<data_type>;
    data_type hessian_estimate = 0;
    unsigned int _cursor = 0;
    size_type _activeBufferSize=0;
    size_type _buffer_size;

    // TODO::implement matrix type
    std::vector<Vec> _s;
    std::vector<Vec> _y;
    std::vector<data_type> _alpha;
    std::vector<data_type> _rho;
    size_type _dimension;

	static int getFloatingIndex(size_type i,size_type cursor, size_type bufferSize) {
		return (cursor - 1 - i + bufferSize) % bufferSize;
	}

public:
    LBFGS(size_type dimension, size_type buffer_size) :
        _dimension(dimension), _alpha(buffer_size), _rho(buffer_size), _buffer_size(buffer_size)
    {
        _activeBufferSize = 0;
        // saves all the stuff column wise, fortran style
		// one element extra used in update
        // very inefficient -> should be done by using some
        // kind of matrix type.
		_s.reserve(buffer_size + 1);
		_y.reserve(buffer_size + 1);
        for(int i = 0; i < buffer_size+1; ++i)
        {
            _s.emplace_back(dimension);
            _y.emplace_back(dimension);
        }
    }

    bool hasCache()
    {
        return _activeBufferSize != 0;
    }

    void Reset()
    {
        _activeBufferSize = 0; 
        _cursor = 0;
    }
    
    void solve(
        const Vec& gradient,
        Vec& outputDirection)
    {
        if (_activeBufferSize == 0) 
        {
            outputDirection = -gradient;
            return;
        }

        outputDirection = gradient*1.0;
        for (size_type absolute_i = 0; absolute_i < _activeBufferSize; ++absolute_i)
        {
            size_type i = getFloatingIndex(absolute_i, _cursor, _buffer_size);

            _rho[i] = 1/(_s[i]*_y[i]);
            _alpha[i] = _rho[i]*(_s[i]*outputDirection);

            outputDirection = outputDirection -(_alpha[i]*_y[i]);
        }

        outputDirection = outputDirection * hessian_estimate;

        data_type beta;
        for (size_type absolute_i = _activeBufferSize-1; absolute_i > -1; --absolute_i)
        {
            size_type i = getFloatingIndex(absolute_i, _cursor, _buffer_size);

            beta = _rho[i]*(_y[i]*outputDirection);
            outputDirection = outputDirection + (_alpha[i]-beta)*_s[i];
        }

        outputDirection = -outputDirection;
    }
    
    template<typename TVectorS,typename TVectorY>
    bool CheckIfValidUpdate(
        const Vec& gradientCurrentLocations,
        const TVectorS& potentialS,
        const TVectorY& potentialY)
    {
        // Theoretical condition:
        // update if (y^Ts)/||s || ^ 2 > epsilon * || grad(x) ||
        data_type value = (potentialS*potentialY)
            / (potentialS*potentialS);

        return value > sqrt(Norm(gradientCurrentLocations))*(1e-12);
    }

    // Update the hessian estimate
    // -> returns true if hessian was adjusted
    // -> returns false if carefull update avoided an hessian update
    bool updateHessian(
        const Vec& currentLocation, 
        const Vec& currentGradient, 
        const Vec& newLocation, 
        const Vec& newGradient)
    {
        auto potentialS = newLocation - currentLocation;
        auto potentialY = newGradient - currentGradient;

        if (this->CheckIfValidUpdate(currentGradient, potentialS, potentialY))
        {
            _s[_cursor] = potentialS;
            _y[_cursor] = potentialY;
    
            hessian_estimate = (potentialS*potentialY)
                / (potentialY*potentialY);

            if (_activeBufferSize < _buffer_size)
            {
                _activeBufferSize++;
            }
            _cursor = (_cursor + 1) % _buffer_size;

            return true;
        }

        return false;
    }
};

}
