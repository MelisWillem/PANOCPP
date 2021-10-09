#include<vector>
#include<memory>
#include<math.h>
#include<limits>
#include<cassert>
#include<iostream>
#include<string>
#include<cstdlib>
#include<utility>
#include<array>
#include<functional>
#include<type_traits>
#include<initializer_list>
#include<sstream>
#include<algorithm>



namespace pnc {
	template <typename TData = double>
	class Vector final {
	private:
		std::vector<TData> data;

	public:
		using data_type = TData;
		using size_type = int;

		Vector(size_type size) : data(size){}

		Vector(std::initializer_list<TData> input) : data(input)
		{
		}

		Vector(const Vector<data_type>& other)
		{
			for (int i = 0; i < size(); ++i)
			{
				data[i] = other[i];
			}
		}

		auto size() const
		{
			return (size_type)data.size();
		}

		constexpr TData operator[](unsigned int index) const
		{
			return data[index];
		}

		constexpr TData& operator[](unsigned int index)
		{
			return data[index];
		}

		Vector(Vector&& other)
			: data(std::move(other.data))
		{
		}

		Vector<TData>& operator=(const Vector<TData>& other)
		{
			for (unsigned int i = 0; i < size(); i++) {
				data[i] = other[i];
			}

			return *this;
		}

		template <
			typename TVecRef,
			typename TVec = std::remove_reference_t<TVecRef>,
			typename TDataTVec = typename TVec::data_type,
			typename TSize = typename TVec::size_type
		>
		Vector<TData>& operator=(TVecRef&& other)
		{
			assert(size() == other.size());
			for (TSize i = 0; i < size(); i++) {
				data[i] = other[i];
			}

			return *this;
		}
	};
}

namespace pnc{

template <typename TSize, typename TData = double>
class VectorUnit final {
private:
    TData constant;
    TSize size_vector;

public:
    TSize size() const {
        return size_vector;
    }
    using data_type = TData;
    using size_type = TSize;

    explicit VectorUnit(TData constant, TSize s): constant(constant), size_vector(s)
    {}

    constexpr auto operator[](unsigned int index) const
    {
        return constant;
    }
};

}


namespace pnc {
    template <typename TVector, typename TMaybeData>
    struct IsDataOfVectorKind {
        static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
    };
}

namespace pnc{
    template <
        typename TVec,
        typename TSize = typename TVec::size_type
        >
    constexpr auto MAX(const TVec& vec)
    {
        assert(vec.size() > 0);
        auto cache = vec[0];
        for (TSize i = 1; i < vec.size(); i++) {
            if (cache < vec[i]) {
                cache = vec[i];
            }
        }
        return cache;
    }

}

namespace pnc{
    template <
        typename TVecRef ,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename TSize = typename TVec::size_type
        >
    constexpr auto SUM(TVecRef&& vec)
    {
        assert(vec.size()> 0);
        auto cache = vec[0];
        for (TSize i = 1; i < vec.size(); i++) {
            cache += vec[i];
        }
        return cache;
    }
}



namespace pnc{

template<typename TData>
struct MaxOperation{
    constexpr auto eval(TData x,TData y) const
    {
        return std::max(x,y);
    }
};

template<typename TData>
struct MulOperation{
    constexpr auto eval(TData x,TData y) const
    {
        return x * y;
    }
};

template<typename TData>
struct AddOperation{
    constexpr auto eval(TData x,TData y) const
    {
        return x + y;
    }
};

template< typename data_type >
struct AbsOp{
     constexpr auto eval(data_type x) const
    {
        return abs(x);
    }
};

template<typename data_type>
struct Negative{
    constexpr auto eval(data_type x)  const
    {
        return -x;
    }
};
}

namespace pnc{
template <
    typename TLeftRef,
    typename TRightRef,
    template<typename> typename PairOp
        >
class VectorPairwiseMap final{
private:
    TLeftRef v1;
    TRightRef v2;

public:
    using TLeft = typename std::remove_reference_t<TLeftRef>;
    using data_type = typename TLeft::data_type;
    using size_type = typename TLeft::size_type;
    size_type vector_size;

    explicit VectorPairwiseMap(
        TLeftRef&& left,
        TRightRef&& right)
        : v1(std::forward<TLeftRef>(left))
        , v2(std::forward<TRightRef>(right)),
        vector_size(left.size())
    {
        assert(left.size() == right.size());
    }

    size_type size() const {
        return vector_size;
    }

    constexpr auto operator[](size_type index) const
    {
        return PairOp<data_type>().eval(v1[index],v2[index]);
    }
};

template< typename TLeftRef, typename TRightRef >
using VectorSum = VectorPairwiseMap<TLeftRef, TRightRef, AddOperation>;

template< typename TLeftRef, typename TRightRef >
using VectorProd = VectorPairwiseMap<TLeftRef,TRightRef,MulOperation>;

template< typename TLeftRef, typename TRightRef >
using VectorComponentMax = VectorPairwiseMap<TLeftRef,TRightRef,MaxOperation>;
}



namespace pnc{
    template <
        typename TVecRef,
        template<typename> typename TFunc,
        typename TVec = typename std::remove_reference_t<TVecRef>
        >
    class VectorMap final {
    private:
        TVecRef v;

    public:
        using data_type = typename TVec::data_type;
        using size_type = typename TVec::size_type;

        VectorMap(TVecRef&& v) 
            : v(std::forward<TVecRef>(v)) {}

        size_type size() const {
            return v.size();
        }

        constexpr auto operator[](size_type index) const
        {
            return TFunc<data_type>().eval(v[index]);
        }
    };

    template<typename TVector>
    using VectorNegative = VectorMap<TVector, Negative>;

    template<typename TVector>
    using VectorAbs = VectorMap<TVector, AbsOp>;
}



namespace pnc{

    template <
        typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename = typename TVec::data_type,
        typename = typename TVec::size_type
            >
    constexpr auto operator-(TVecRef&& vec)
    {
        return VectorNegative<TVecRef>(std::forward<TVecRef>(vec));
    }

    // vector + constant
    template <
        typename TLeftRef,
        typename TRight,
        typename TLeft = typename std::remove_reference_t<TLeftRef>,
        typename = typename TLeft::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
    constexpr auto operator+(
        TLeftRef&& left,
        TRight right)
    {
        using TRightVec = VectorUnit<typename TLeft::size_type, typename TLeft::data_type>;
        return VectorSum<TLeftRef, TRightVec>
            (std::forward<TLeftRef>(left), TRightVec(right, left.size()));
    }

    // constant + vector
    template <
        typename TLeft,
        typename TRightRef,
        typename TRight = typename std::remove_reference_t<TRightRef>,
        typename = typename TRight::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
    constexpr auto operator+(
        TLeft left, // not a vector
        TRightRef&& right)
    {
        using TLeftVector = VectorUnit<typename TRight::size_type, typename TRight::data_type>;
        return VectorSum<TLeftVector,TRightRef>
            (TLeftVector(left, right.size()),std::forward<TRightRef>(right));
    }

    // vector * constant
    template <
        typename TLeftRef,
        typename TRight,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename = typename TLeft::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
    auto operator*(
        TLeftRef&& left,
        TRight right)
    {
        using TRightVec = VectorUnit<typename TLeft::size_type, typename TLeft::data_type>;
        return VectorProd<TLeftRef,TRightVec>(std::forward<TLeftRef>(left),TRightVec(right, left.size()));
    }

    // constant * vector
    template <
        typename TLeft,
        typename TRightRef,
        typename TRight = std::remove_reference_t<TRightRef>,
        typename = typename TRight::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
    constexpr auto operator*(
        TLeft left,
        TRightRef&& right)
    {
        using TLeftVector = VectorUnit<typename TRight::size_type,typename TRight::data_type>;
        return VectorProd<TLeftVector,TRightRef>(TLeftVector(left,right.size()),std::forward<TRightRef>(right));
    }

    template <
        typename TLeftRef,
        typename TRightRef,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename TRight = std::remove_reference_t<TRightRef>,
        typename typeL = typename TLeft::data_type,
        typename typeR = typename TRight::data_type
            >
    constexpr auto operator-(
        TLeftRef&& left,
        TRightRef&& right)
    {
        return left + (-right);
    }

    template <
        typename TLeftRef,
        typename TRightRef,
        typename TRight = std::remove_reference_t<TRightRef>,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename typeL = typename TLeft::data_type,
        typename typeR = typename TRight::data_type,
        typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>
        >
    constexpr auto operator+(
        TLeftRef&& left,
        TRightRef&& right)
    {
        assert(left.size() == right.size());
        return VectorSum<TLeftRef,TRightRef>(
            std::forward<TLeftRef>(left),
            std::forward<TRightRef>(right));
    }

    template <
        typename TLeftRef,
        typename TRightRef,
        typename TLeft = typename std::remove_reference_t<TLeftRef>,
        typename TRight = typename std::remove_reference_t<TRightRef>,
        typename typeL = typename TLeft::data_type,
        typename typeR = typename TRight::data_type,
        typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>
        >
    constexpr auto operator*(
        TLeftRef&& left,
        TRightRef&& right)
    {
        return SUM(
            VectorProd<TLeftRef, TRightRef>(
                std::forward<TLeftRef>(left),
                std::forward<TRightRef>(right)));
    }
}

namespace pnc{

    template <
        typename TLeftRef,
        typename TRightRef
        >
    constexpr auto ComponentWiseMax(
        TLeftRef&& left,
        TRightRef&& right)
    {
        return VectorComponentMax<TLeftRef, TRightRef>(
                std::forward<TLeftRef>(left),
                std::forward<TRightRef>(right));
    }

    template <
        typename TVector,
        typename data_type = typename TVector::data_type
        >
    constexpr data_type Norm(TVector& v)
    {
        return (v * v);
    }

    template <
        typename TVector,
        typename TSQRT,
        typename data_type = typename TVector::data_type
        >
    constexpr auto Norm2(TVector& v,TSQRT sqrtOperator)
    {
        return sqrtOperator(v * v);
    }

    template<
        typename TVectorRef,
        typename TVector = std::remove_reference_t<TVectorRef>,
        typename data_type = typename TVector::data_type
        >
    constexpr auto NormL1(TVectorRef&& vec) 
    {
        return SUM(VectorAbs<TVectorRef>(std::forward<TVectorRef>(vec)));
    }
}



namespace pnc {
	template<
		typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
		typename TData = typename TVec::data_type,
        typename TSize = typename TVec::size_type
	>
		TData InfNorm(TVecRef&& vec)
	{
		TData max = std::numeric_limits<TData>::min();

		for (TSize i = 0; i < vec.size(); ++i)
		{
			if (max < vec[i]) { max = vec[i]; }
		}

		return max;
	}
}// Proximal operator should look like:
// x_new = prox_op(x_old)
// This should use the concept of a vector and
// not the vector itself.



namespace pnc {

// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of
// the closest border is returned.
// precondition: low < high
template <
    typename TVectorRef,
    typename TVector = typename std::remove_reference_t<TVectorRef>,
    typename TSize = typename TVector::size_type,
    typename TData = typename TVector::data_type
>
class BoxOperator final {
public:
    using data_type = TData;
    using size_type = TSize;

private:
    const TVectorRef input;
    const data_type penalty = 1; // todo make this dyamic...
    const data_type low;
    const data_type high;
    data_type _cost;

public:
    explicit BoxOperator(
        TVectorRef&& input,
        const data_type low,
        const data_type high)
        : input(std::forward<TVectorRef>(input))
        , low(low)
        , high(high)
    {
        _cost = 0;
        for (size_type i = 0; i < input.size(); ++i) {
            if (input[i] > high || input[i] < low) {
                _cost += penalty;
            }
        }
    }

    constexpr auto operator[](const int index)
    {
        if (input[index] > high) {
            return high;
        }
        if (input[index] < low) {
            return low;
        }
        return input[index];
    }

    constexpr auto cost() const
    {
        return _cost;
    }

    auto size() const
    {
        return input.size();
    }
};

// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of
// the closest border is returned.
template<
    typename TVectorRef,
    typename TVector = std::remove_reference_t<TVectorRef>
>
constexpr auto Box(
    TVectorRef&& input,
    const typename TVector::data_type low,
    const typename TVector::data_type high)
{
    return BoxOperator<decltype(input)>
        (std::forward<TVectorRef>(input), low, high);
}

template <
    typename data_type
>
struct BoxOp final {
    const data_type low;
    const data_type high;
    BoxOp(
        const data_type low,
        const data_type high)
        : low(low)
        , high(high)
    {
    }

    template<
        typename TVecRef,
        typename TVec = typename std::remove_reference_t<TVecRef>,
        typename = typename std::enable_if_t<std::is_same<data_type, typename TVec::data_type>::value>
    >
    auto operator()(TVecRef&& input) const
    {
        return BoxOperator<TVecRef>
            (std::forward<TVecRef>(input), low, high);
    }
};

}


namespace pnc
{
    template<
        typename TVecRef,
        typename TVec = typename std::remove_reference_t<TVecRef>,
        typename TData = typename TVec::data_type,
        typename TSize = typename TVec::size_type
            >
    class NormBoxOperator{
		public:
            using data_type = TData;
            using size_type = TSize;
        private: 
            enum State{
                low,
                mid,
                high
            };

            TVecRef _input;
            State _state;
            data_type _cost;
            data_type offset;

            constexpr auto sign(const data_type x) const
            {
                return x < data_type{0} 
                ? data_type{-1} 
                : data_type{1};
            }

        public:
            NormBoxOperator(TVecRef&& vec,data_type offset)
                : _input(std::forward<TVecRef>(vec)),offset(offset)
            {
                auto norml1 = NormL1(vec);
                if(norml1<offset)
                {
                    _state = State::low;
                }
                else if( norml1 > 2*offset)
                {
                    _state = State::high;
                }
                else
                {
                    _state = State::mid;
                }
                _cost = std::max(data_type{0},norml1-offset);
            }

            constexpr auto operator[](const unsigned int index) const
            {
                switch(_state)
                {
                    case State::low:
                        return _input[index];
                    case State::mid:
                        return sign(_input[index])*offset;
                    default: // case State::high:
                        return sign(_input[index])*(abs(_input[index])-offset);
                }
            }

            constexpr auto cost() const
            {
                return _cost;
            }

            constexpr auto size() const
            {
                return _input.size();
            }
    };

    template< typename data_type >
    struct NormBox{
        const data_type offset;
        NormBox(data_type offset) : offset(offset)
        {
        }

        template<
            typename TVecRef,
            typename TVec = std::remove_reference_t<TVecRef>,
            typename = std::enable_if_t<std::is_same_v<typename TVec::data_type,data_type>>
                >
        auto operator()(TVecRef&& vec) const
        {
            return NormBoxOperator<TVecRef>(std::forward<TVecRef>(vec),offset);
        }
    };
}



namespace pnc {

	template <
		typename TVectorRef,
		typename TVector = std::remove_reference_t<TVectorRef>,
		typename TConstant = typename TVector::data_type,
		typename TSize = typename TVector::size_type
	>
		struct Location {
		Location(
			TVectorRef&& location,
			TVectorRef&& gradient,
			TConstant cost,
			TConstant gamma) :
			location(std::forward<TVectorRef>(location)),
			gradient(std::forward<TVectorRef>(gradient)),
			gamma(gamma),
			cost(cost)
		{ }

		Location(TSize dimension) :
			location(dimension),
			gradient(dimension),
			gamma(0),
			cost(0)
		{
		}

		template<typename TVec>
		Location(const Location<TVec>& other)
			:
			location(other.location),
			gradient(other.gradient),
			gamma(other.gamma),
			cost(other.cost)
		{
		}

		template<typename TVec>
		Location<TVec>& operator=(const Location<TVec>& other)
		{
			location = other.location;
			gradient = other.gradient;
			gamma = other.gamma;
			 cost = other.cost;

			return this;
		}

		TVectorRef location;
		TVectorRef gradient;
		TConstant gamma;
		TConstant cost;
	};

}


namespace pnc {

class LipschitzEstimator{
public:
    struct Config {
        // The lipschitz value is estimated over an delta of:
        // delta= max{MinimumDelta,LipschitzSafetyValue*u_0}
        
        const double lipschitz_safetyValue; // default in C# is: 1e-6
        const double minimum_delta;         // default in C# is: 1e-12
    };
    static constexpr Config default_config =
    {
        1e-6, // lipschitz_safetyValue
        1e-12 // minimum_delta
    };

    // Estimate the lipschitz constant by using the numerical hessian as an estimation
    //   Theorem:
    //      ||gradient(x)|| < B
    //       f is B-lipschitz
    // => cache: Memory location used to evaluate slightly deviated cost/gradient,
    //           -> Assume it's poluted after this call, and do NOT use the content.
    //           -> This avoids allocating an additional vector that is potentially large.
    template<
        typename TVector,
        typename TConfig,
        typename TCostFunction,
        typename data_type = typename TVector::data_type,
        typename size_type = typename TVector::size_type
            >
    static data_type estimate(
        const Location<TVector>& location,
        const TConfig& config,
        TCostFunction& cost_function,
        TVector& cache) 
    {
        // Find delta= max{small number,10^{-6}*u_0}
        auto delta = ComponentWiseMax(
                (config.lipschitz_safetyValue*location.location),
                pnc::VectorUnit<size_type, data_type>(config.minimum_delta, location.location.size()));

        auto deviated_position = location.location + delta;
        auto& deviated_gradient = cache;
        cost_function(deviated_position, deviated_gradient);

        auto buff = location.gradient-deviated_gradient;
        return sqrt(buff * buff)/
            sqrt(delta*delta);
    }
};
}

template<typename T>
std::string ToString(const pnc::Vector<T>& vec)
{
    if (vec.size() == 0)
    {
		return "[]";
    }
    if (vec.size() == 1) {
		return "[" + std::to_string(vec[0]) + "]";
    }

    std::stringstream ss;
    ss << "[";
    const auto dimension = vec.size();
    for(typename T::size_type i=0; i < dimension-1; ++i)
    {
        ss << std::to_string(vec[i]) << ", ";
    }
    ss << std::to_string(vec[dimension-1]);
    ss << "]";


    return ss.str();
}

namespace pnc {

	// Contains a proximal gradient step:
	// -. Does not own the vector's itself !
	template<typename TVec>
	struct ProximalGradientStep {
		ProximalGradientStep(
			Location<TVec>& c,
			Location<TVec>& p) :
			current(c),
			proximal(p) {}

		Location<TVec>& current;
		Location<TVec>& proximal;

	};

	template<
		typename TCostFunction,
		typename TProximalOperator
	>
		class ProximalCalculator {
		public:
			using Estimator = LipschitzEstimator;
			TProximalOperator& prox_;

			struct Config {
				/// The linesearch condition should be :
				// fNew > f - df.DotProduct(df) 
				//     + (1 ) / (2 * new_location.Gamma) * (direction.DotProduct(direction))
				//
				// In reality it is:
				// fNew > f - df.DotProduct(df)
				//     + (1 - safetyValueLineSearch) / (2 * new_location.Gamma) * directionSquaredNorm
				//     + 1e-6 * f;
				const double safety_value_line_search;
				// The linesearch parameter gamma can never be smaller then this
				const double min_gamma_value;
			};
			static constexpr Config default_config =
			{
				0.05, // safety_value_line_search
				1e-15 // min_gamma_value
			};

			TCostFunction& cost_function_;

			ProximalCalculator(TCostFunction& cost_function, TProximalOperator& prox) :
				cost_function_(cost_function),
				prox_(prox) {}

			template<
				typename TVector,
				typename data_type = typename TVector::data_type
			>
				void Calculate(
					const Location<TVector>& current,
					Location<TVector>& proximal,
					const Config config)
			{
				TakeProxStep(
					current,
					current.gamma,
					proximal);

				while (!LinesearchCondition(current, proximal, config.safety_value_line_search) &&
					(proximal.gamma / data_type{ 2 } > config.min_gamma_value))
				{
					TakeProxStep(
						current,
						proximal.gamma / data_type{ 2 },
						proximal);
				}
			}

		private:
			// use the value of current_location to calculate a
			// new proximal gradient step.
			template<
				typename TVector,
				typename data_type = typename TVector::data_type
			>
				void TakeProxStep(
					const Location<TVector>& current,
					const data_type gamma,
					Location<TVector>& proximal) const
			{
				proximal.gamma = gamma;
				proximal.location = prox_(current.location - gamma * current.gradient);
				proximal.cost = cost_function_(
					proximal.location,
					proximal.gradient);
			}

			template<
				typename TVector,
				typename data_type = typename TVector::data_type
			>
				bool LinesearchCondition(
					const Location<TVector>& current,
					const Location<TVector>& proximal,
					const data_type safety_value_line_search)  const
			{
				const auto direction_squared_norm = (current.location - proximal.location)
					* (current.location - proximal.location);

				const auto f = current.cost;
				const auto& df = current.gradient;
				const auto f_new = proximal.cost;
				//const auto& df_new = proximal.gradient;

				return f_new > f - df * df
					+ (1 - safety_value_line_search) / (2 * proximal.gamma)
					* direction_squared_norm
					+ 1e-6 * f;
			}
	};

}


namespace pnc{


template<
    int buffer_size,
    typename data_type,
    typename size_type
        >
class LBFGS
{
private:
    using Vec = Vector<data_type>;
    data_type hessian_estimate = 0;
    unsigned int _cursor = 0;
    unsigned int _activeBufferSize=0;

    // TODO::implement matrix type
    std::vector<Vec> _s;
    std::vector<Vec> _y;
    data_type _alpha[buffer_size];
    data_type _rho[buffer_size];
    size_type _dimension;

	static int getFloatingIndex(int i,int cursor,int bufferSize) {
		return (cursor - 1 - i + bufferSize) % bufferSize;
	}

public:
    LBFGS(size_type dimension) :
        _dimension(dimension)
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
        for (int absolute_i = 0; absolute_i < _activeBufferSize; ++absolute_i)
        {
            unsigned int i = getFloatingIndex(absolute_i, _cursor, buffer_size);

            _rho[i] = 1/(_s[i]*_y[i]);
            _alpha[i] = _rho[i]*(_s[i]*outputDirection);

            outputDirection = outputDirection -(_alpha[i]*_y[i]);
        }

        outputDirection = outputDirection * hessian_estimate;

        data_type beta;
        for (int absolute_i = _activeBufferSize-1; absolute_i > -1; --absolute_i)
        {
            unsigned int i = getFloatingIndex(absolute_i, _cursor, buffer_size);

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

            if (_activeBufferSize < buffer_size)
            {
                _activeBufferSize++;
            }
            _cursor = (_cursor + 1) % buffer_size;

            return true;
        }

        return false;
    }
};

}

namespace pnc{

template<typename TCost, typename TConstraint>
struct FBE
{
    TCost& cost_function_;
    TConstraint& constraint_;

	FBE(TCost& cost_function, TConstraint& constraint)
        : cost_function_(cost_function),constraint_(constraint)
    {
    }

    // calculate the forward backward envelop using the internal gamma
    // Matlab cache.FBE = cache.fx + cache.gz - cache.gradfx(:)'*cache.FPR(:)
    // + (0.5/gam)*(cache.normFPR^2);
    template<typename TVec, typename TData = typename TVec::data_type>
    TData Eval(const Location<TVec>& current, Location<TVec>& proximal)
    {
        auto direction = current.location - proximal.location;
        auto gz = constraint_(proximal.location).cost(); 
        auto fbe = current.cost
            + gz
            - (current.gradient*(direction))
            + (1/ (proximal.gamma*2))*(direction*direction);

        return fbe;
    }
};

}


namespace pnc{

    template<
        typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename data_type = typename TVec::data_type
        >
    struct LocationBuilder{

        template<typename TCostFunction>
        static Location<TVec> Build(
                TCostFunction& cost_function,
                TVec&& position,
                TVec&& gradient,
                data_type cost,
                TVec& cache)
        {
            using Estimator = LipschitzEstimator;
            auto config = Estimator::default_config;
            Location<TVec> location(
                    std::forward<TVecRef>(position),
                    std::forward<TVecRef>(gradient),
                    cost,
                    0);
            double safety_value_linesearch = 0.05;
            location.gamma = (1-safety_value_linesearch)/Estimator::estimate(
                    location,
                    config,
                    cost_function,
					cache);

            return location;
        }
    };
}

namespace pnc {
	struct PanocConfig {
		int max_iterations;
		int max_fbe_iterations;
		double min_residual;
	};

	template<typename TCostFunc, typename TProx>
	class Panoc {

		TCostFunc& cost_function_;
		TProx& prox_;
		PanocConfig config_;
		pnc::ProximalCalculator<TCostFunc, TProx> prox_calc_;
		typename pnc::ProximalCalculator<TCostFunc, TProx>::Config prox_calc_config_ 
			= pnc::ProximalCalculator<TCostFunc, TProx>::default_config;
		pnc::FBE<TCostFunc, TProx> fbe_;
		// TODO: make double/int generic
		LBFGS<10, double, int> accelerator_;

	public:
		// TODO:: make in generic
		Panoc(TCostFunc& cost_function, TProx& prox, PanocConfig& config, int dimension)
			: cost_function_(cost_function),
			prox_(prox),
			config_(config),
			prox_calc_(cost_function_, prox_),
			fbe_(cost_function, prox_),
			accelerator_(dimension)
		{}

		template<typename TVec>
		void Solve(TVec& input)
		{
			TVec vector = input;
			TVec gradient(input.size()); // will be moved.
			double fbe = 0;
			double residual = std::numeric_limits<double>::max();

			double cost = cost_function_(vector, gradient);

			TVec cache(input.size()); // Used only once when estimating gamma the first time.
			Location<TVec> current = LocationBuilder<TVec>().Build(
				cost_function_,
				std::move(vector),
				std::move(gradient),
				cost,
				cache
			);

			auto prox_config = decltype(prox_calc_)::default_config;
			Location<TVec> current_old = current;
			Location<TVec> proximal(input.size());
			prox_calc_.Calculate(current, proximal, prox_config);

			for (int i = 0; i < config_.max_iterations && residual>config_.min_residual; ++i)
			{
				auto oldGamma = proximal.gamma;
				if (accelerator_.hasCache()) // If there is accelstep(which needs previous runs) then we can improve stuff
				{
					TVec accelerator_step(input.size());
					accelerator_.solve(current.gradient, accelerator_step);
					current_old = current;
					std::tie(residual, fbe) = Search(current, proximal, fbe, accelerator_step);
				}
				else
				{
					std::swap(current, proximal);
					prox_calc_.Calculate(current, proximal, decltype(prox_calc_)::default_config);
					fbe = fbe_.Eval(current, proximal);
					residual = ResidualInfNorm(current, proximal);
				}

				// This update doesn't always mean that the cache will be updated,
				// the lbfgs does a carefull update and will refuse some updates due to beeing badly conditioned
				if (oldGamma != proximal.gamma) { accelerator_.Reset(); }
				bool cache_updated = accelerator_.updateHessian(
					current_old.location,
					current_old.gradient,
					current.location,
					current.gradient);
			}

			input = current.location;
		}

		template<typename TVec>
		double ResidualInfNorm(const Location<TVec>& current,const Location<TVec>& proximal)
		{
			return InfNorm((current.location - proximal.location) * (1/proximal.gamma));
		}

		template<typename TVec>
		std::tuple<double, double> Search(
			Location<TVec>& current,
			Location<TVec>& proximal,
			double fbe,
			TVec& acceleration_step)
		{
			auto tau = [](auto i) {return pow(2, i); };
			// linesearch on fbe, if the condition is never met. Use
			// as pure proximal gradient step.
			for (int i = 0; i < config_.max_fbe_iterations; ++i)
			{
				Location<TVec> potential_new_location(current.location.size());
				Location<TVec> potential_new_prox_location(current.location.size());
				// Take a step away from the current location, using part proximal
				// and part the accerator.
				potential_new_location.location = current.location
					+ (proximal.location - current.location) * (1 - tau(i))
					+ acceleration_step * tau(i);
				potential_new_location.cost = cost_function_(potential_new_location.location, potential_new_location.gradient);
				double safety_value_linesearch = 0.05;
				TVec cache(current.location.size());
				auto lip_config = LipschitzEstimator::default_config;
				potential_new_location.gamma = (1-safety_value_linesearch)/LipschitzEstimator::estimate<
					TVec,
					decltype(lip_config),
					decltype(cost_function_)>(
						potential_new_location,
						lip_config,
						cost_function_,
						cache);

				// Calculate the fbe use a proximal step, keep the proximal step in the cache
				// as we can reuse it if this step is accepted.
				prox_calc_.Calculate(potential_new_location, potential_new_prox_location, prox_calc_config_);
				auto new_fbe = fbe_.Eval(potential_new_location, potential_new_prox_location);

				if (new_fbe < fbe)
				{
					// Accept potential new location/prox_location
					const auto res = ResidualInfNorm(potential_new_location, potential_new_prox_location);
					std::swap(potential_new_location, current); 
					std::swap(potential_new_prox_location, proximal); 
					return { res,new_fbe };
				}
			}

			// use only proximal gradient, no accelerator
			auto prox_config = decltype(prox_calc_)::default_config;
			std::swap(current, proximal); // Take a pure prox step.
			prox_calc_.Calculate(current,proximal, prox_config);
			const double res = ResidualInfNorm(current, proximal);
			const double new_fbe = fbe_.Eval(current, proximal);
			return { res, new_fbe };
		}
	};
}
