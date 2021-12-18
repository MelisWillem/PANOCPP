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
    size_type _cursor = 0;
    size_type _activeBufferSize=0;
    size_type _cache_size;

    Matrix<data_type, size_type> _s;
    Matrix<data_type, size_type> _y;
    Vector<data_type, size_type> _alpha;
    Vector<data_type, size_type> _rho;
    size_type _dimension;

	static int getFloatingIndex(size_type i,size_type cursor, size_type cache_size) {
		return (cursor - 1 - i + cache_size) % cache_size;
	}

public:
    LBFGS(size_type dimension, size_type cache_size) :
        _dimension(dimension),
        _alpha(cache_size),
        _rho(cache_size),
        _cache_size(cache_size),
        _s(cache_size, dimension),
        _y(cache_size, dimension)
    {
        _activeBufferSize = 0;
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
        Vec& output_direction)
    {
        if (_activeBufferSize == 0) 
        {
            output_direction = -gradient;
            return;
        }

        output_direction = gradient*1.0;
        for (size_type absolute_i = 0; absolute_i < _activeBufferSize; ++absolute_i)
        {
            size_type i = getFloatingIndex(absolute_i, _cursor, _cache_size);

            _rho[i] = 1/(_s[i]*_y[i]);
            _alpha[i] = _rho[i]*(_s[i]*output_direction);

            output_direction = output_direction -(_alpha[i]*_y[i]);
        }

        output_direction = output_direction * hessian_estimate;

        data_type beta;
        for (size_type absolute_i = _activeBufferSize-1; absolute_i > -1; --absolute_i)
        {
            size_type i = getFloatingIndex(absolute_i, _cursor, _cache_size);

            beta = _rho[i]*(_y[i]*output_direction);
            output_direction = output_direction + (_alpha[i]-beta)*_s[i];
        }

        output_direction = -output_direction;
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

            if (_activeBufferSize < _cache_size)
            {
                _activeBufferSize++;
            }
            _cursor = (_cursor + 1) % _cache_size;

            return true;
        }

        return false;
    }
};

}
