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



namespace pnc {
	template <unsigned int TSize, typename TData = double>
	class Vector final {
	private:
		std::array<TData, TSize> data;

	public:
		static constexpr unsigned int size = TSize;
		using data_type = TData;

		Vector() : data(){}

		Vector(std::initializer_list<TData> input)
		{
			size_t i = 0;
			for (const auto val : input)
			{
				data[i] = val;
				++i;
			}
		}

		Vector(const Vector<size, data_type>& other)
		{
			for (int i = 0; i < size; ++i)
			{
				data[i] = other[i];
			}
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

		Vector<size, TData>& operator=(const Vector<size, TData>& other)
		{
			for (unsigned int i = 0; i < size; i++) {
				data[i] = other[i];
			}

			return *this;
		}

		template <
			typename TVecRef,
			typename TVec = std::remove_reference_t<TVecRef>>
		Vector<size, TData>& operator=(TVecRef&& other)
		{
			static_assert(
				size == TVec::size,
				"Trying to assign vector expression to a vector of a different dimension");
			for (unsigned int i = 0; i < size; i++) {
				data[i] = other[i];
			}

			return *this;
		}
	};
}

namespace pnc{

template <unsigned int TSize, typename TData = double>
class VectorUnit final {
private:
    TData constant;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    explicit VectorUnit(TData constant): constant(constant)
    {}

    constexpr auto operator[](unsigned int index) const
    {
        return constant;
    }
};

}

// Extra<helper stuff to keep things clean

namespace pnc {
    template <typename TVector, typename TMaybeData>
    struct IsDataOfVectorKind {
        static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
    };
}

namespace pnc{
    template <
        typename TVec,
        unsigned int size = TVec::size
        >
    constexpr auto MAX(const TVec& vec)
    {
        static_assert(size> 0,"Max of a vector is only defined if the dimension of the vector is larger then 0");
        auto cache = vec[0];
        for (unsigned int i = 1; i < vec.size; i++) {
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
        unsigned int size = TVec::size
        >
    constexpr auto SUM(TVecRef&& vec)
    {
        static_assert(size> 0,"Sum of a vector is only defined if the dimension of the vector is larger then 0");
        auto cache = vec[0];
        for (unsigned int i = 1; i < vec.size; i++) {
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
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    explicit VectorPairwiseMap(
        TLeftRef&& left,
        TRightRef&& right)
        : v1(std::forward<TLeftRef>(left))
        , v2(std::forward<TRightRef>(right))
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return PairOp<data_type>().eval(v1[index],v2[index]);
    }
};

template< typename TLeftRef, typename TRightRef >
using VectorSum = VectorPairwiseMap<TLeftRef,TRightRef,AddOperation>;

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
        static constexpr unsigned int size = TVec::size;
        using data_type = typename TVec::data_type;

        VectorMap(TVecRef&& v) 
            : v(std::forward<TVecRef>(v)) {}

        constexpr auto operator[](unsigned int index) const
        {
            return TFunc<data_type>().eval(v[index]);
        }
    };

    template<typename TVector>
    using VectorNegative = VectorMap<TVector,Negative>;

    template<typename TVector>
    using VectorAbs = VectorMap<TVector,AbsOp>;
}


namespace pnc{

    template <typename TVecRef, typename TVec = std::remove_reference_t<TVecRef>>
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
        using TRightVec = VectorUnit<TLeft::size,typename TLeft::data_type>;
        return VectorSum<TLeftRef,TRightVec>(std::forward<TLeftRef>(left),TRightVec(right));
    }

    // constant + vector
    template <
        typename TLeft,
        typename TRightRef,
        typename TRight = typename std::remove_reference_t<TRightRef>,
        typename = typename TRight::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
    constexpr auto operator+(
        TLeft left,
        TRightRef&& right)
    {
        using TLeftVector = VectorUnit<TRight::size,typename TRight::data_type>;
        return VectorSum<TLeftVector,TRightRef>
            (TLeftVector(left),std::forward<TRightRef>(right));
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
        using TRightVec = VectorUnit<TLeft::size,typename TLeft::data_type>;
        return VectorProd<TLeftRef,TRightVec>(std::forward<TLeftRef>(left),TRightVec(right));
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
        using TLeftVector = VectorUnit<TRight::size,typename TRight::data_type>;
        return VectorProd<TLeftVector,TRightRef>(TLeftVector(left),std::forward<TRightRef>(right));
    }

    template <
        typename TLeft,
        typename TRight>
    auto operator-(
        TLeft&& left,
        TRight&& right)
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
        typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
        unsigned int sizeL = TLeft::size,
        unsigned int sizeR = TRight::size,
        typename = typename std::enable_if_t<sizeL==sizeR>
        >
    constexpr auto operator+(
        TLeftRef&& left,
        TRightRef&& right)
    {
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
        typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
        unsigned int sizeL = TLeft::size,
        unsigned int sizeR = TRight::size,
        typename = typename std::enable_if_t<sizeL==sizeR>
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
        typename TRightRef,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename TRight = std::remove_reference_t<TRightRef>,
        unsigned int left_size = TLeft::size,
        unsigned int right_size = TRight::size,
        typename = typename std::enable_if_t<left_size==right_size>
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
        unsigned int size = TVector::size,
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
        unsigned int size = TVec::size
	>
		TData InfNorm(TVecRef&& vec)
	{
		TData max = std::numeric_limits<TData>::min();

		for (int i = 0; i < size; ++i)
		{
			if (max < vec[i]) { max = vec[i]; }
		}

		return max;
	}
}
namespace pnc{

template<typename TVector>
struct VectorTraits;

template<typename TData,auto TSize>
struct VectorTraits<pnc::Vector<TSize, TData>>
{
    using DataType = TData;
    static constexpr auto size = TSize;
};

template<typename TVec, template<typename> typename TPolicy>
struct VectorTraits<pnc::VectorMap<TVec, TPolicy>>{
    using DataType = typename VectorTraits<TVec>::DataType;
    static constexpr auto size = VectorTraits<TVec>::size;
};

template<auto TSize, typename TData>
struct VectorTraits<pnc::VectorUnit<TSize, TData>>{
    using DataType = TData;
    static constexpr auto size = TSize;
};

template<
    typename TVecLeft,
    typename TVecRight,
    template<typename> typename TPairPolicy
>
struct VectorTraits<VectorPairwiseMap<TVecLeft, TVecRight, TPairPolicy>>{
    // We should also make sure that the left and right vector
    // have the same size and datatype when this trait is called.
    using DataType = typename VectorTraits<TVecLeft>::DataType;
    static constexpr auto size = VectorTraits<TVecLeft>::size;
};

}



namespace pnc {

	template <
		typename TVectorRef,
		typename TVector = std::remove_reference_t<TVectorRef>,
		typename TConstant = typename TVector::data_type
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

		Location() :
			location(),
			gradient(),
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
        typename data_type = typename TVector::data_type
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
                pnc::VectorUnit<2,double>(config.minimum_delta));

        auto deviated_position = location.location + delta;
        auto& deviated_gradient = cache;
        cost_function(deviated_position, deviated_gradient);

        auto buff = location.gradient-deviated_gradient;
        return sqrt(buff * buff)/
            sqrt(delta*delta);
    }
};
}

template<
    typename T,
    auto dimension,
    typename = typename std::enable_if_t<std::less<int>()(1, dimension)>>
std::string ToString(const pnc::Vector<dimension, T>& vec)
{
    std::stringstream ss;
    ss << "[";
    for(std::size_t i=0; i < dimension-1; ++i)
    {
        ss << std::to_string(vec[i]) << ", ";
    }
    ss << std::to_string(vec[dimension-1]);
    ss << "]";


    return ss.str();
}

template<typename T>
std::string ToString(const pnc::Vector<1, T>& vec)
{
    return "[" + std::to_string(vec[0]) + "]";
}

template<typename T>
std::string ToString(const pnc::Vector<0, T>& vec)
{
    return "[]";
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


template<int buffer_size,typename data_type, int dimension>
class LBFGS
{
private:
    using Vec = Vector<dimension,data_type>;
    data_type hessian_estimate = 0;
    unsigned int _cursor = 0;
    unsigned int _activeBufferSize=0;

    Vec _s[buffer_size + 1]; // saves all the stuff column wise, fortran style
    Vec _y[buffer_size + 1]; // one element extra used in update
    data_type _alpha[buffer_size];
    data_type _rho[buffer_size];

	static int getFloatingIndex(int i,int cursor,int bufferSize) {
		return (cursor - 1 - i + bufferSize) % bufferSize;
	}

public:
    LBFGS()
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
		LBFGS<10, double, 2> accelerator_;

	public:
		Panoc(TCostFunc& cost_function, TProx& prox, PanocConfig& config)
			: cost_function_(cost_function),
			prox_(prox),
			config_(config),
			prox_calc_(cost_function_, prox_),
			fbe_(cost_function, prox_),
			accelerator_()
		{}

		template<typename TVec>
		void Solve(TVec& input)
		{
			TVec vector = input;
			TVec gradient; // will be moved.
			double fbe = 0;
			double residual = std::numeric_limits<double>::max();

			double cost = cost_function_(vector, gradient);

			TVec cache; // Used only once when estimating gamma the first time.
			Location<TVec> current = LocationBuilder<TVec>().Build(
				cost_function_,
				std::move(vector),
				std::move(gradient),
				cost,
				cache
			);

			auto prox_config = decltype(prox_calc_)::default_config;
			Location<TVec> current_old = current;
			Location<TVec> proximal;
			prox_calc_.Calculate(current, proximal, prox_config);

			for (int i = 0; i < config_.max_iterations && residual>config_.min_residual; ++i)
			{
				auto oldGamma = proximal.gamma;
				if (accelerator_.hasCache()) // If there is accelstep(which needs previous runs) then we can improve stuff
				{
					TVec accelerator_step;
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
				Location<TVec> potential_new_location;
				Location<TVec> potential_new_prox_location;
				// Take a step away from the current location, using part proximal
				// and part the accerator.
				potential_new_location.location = current.location
					+ (proximal.location - current.location) * (1 - tau(i))
					+ acceleration_step * tau(i);
				potential_new_location.cost = cost_function_(potential_new_location.location, potential_new_location.gradient);
				double safety_value_linesearch = 0.05;
				TVec cache;
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
