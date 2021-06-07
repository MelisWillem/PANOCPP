#include"vectorAlgebra.hpp"
#include"ProximalCalculator.hpp"
#include"LBFGS.hpp"
#include<math.h>

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
		typename pnc::ProximalCalculator<TCostFunc, TProx>::Config prox_calc_config_ = pnc::ProximalCalculator<TCostFunc, TProx>::default_config;
		pnc::FBE<TCostFunc, TProx> fbe_;
		LBFGS<10, double, 2> accelerator_;

	public:
		Panoc(TCostFunc cost_function, TProx& prox, PanocConfig& config)
			: cost_function_(cost_function),
			prox_(prox),
			config_(config),
			prox_calc_(cost_function_, prox_),
			fbe_(cost_function, prox_),
			accelerator_()
		{
		}

		template<typename TVec>
		void Solve(TVec& vector)
		{
			TVec gradient; // will be moved.
			TVec cache; // will be moved.
			double fbe;
			double residual = std::numeric_limits<double>::max();

			double cost = cost_function_(vector, gradient);

			auto current = LocationBuilder<TVec>().Build(
				cost_function_,
				std::move(vector),
				std::move(gradient),
				cost,
				cache
			);

			auto prox_config = decltype(prox_calc_)::default_config;
			Location<TVec> proximal;
			prox_calc_.Calculate(current, proximal, prox_config);

			for (int i = 0; i < config_.max_iterations && residual>config_.min_residual; ++i)
			{
				auto oldGamma = proximal.gamma;
				if (accelerator_.hasCache()) // If there is accelstep(which needs previous runs) then we can improve stuff
				{
					TVec accelerator_step;
					accelerator_.solve(current.gradient, accelerator_step);
					std::tie(residual, fbe) = Search(current, proximal, fbe, accelerator_step);
				}
				else
				{
					std::swap(current, proximal);
					prox_calc_.Calculate(current, proximal, decltype(prox_calc_)::default_config);
					fbe = fbe_.Eval(current, proximal);
					// todo:: do we need to set the res here???
				}

				// This update doesn't always mean that the cache will be updated,
				// the lbfgs does a carefull update and will refuse some updates due to beeing badly conditioned
				if (oldGamma != proximal.gamma) { accelerator_.Reset(); }
				bool cache_updated = accelerator_.updateHessian(
					current.location,
					current.gradient,
					proximal.location,
					proximal.gradient);
			}
		}

		template<typename TVec>
		double ResidualInfNorm(TVec vec)
		{
			return 0;
		}

		// return {residual, fbe}
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
				potential_new_location.gamma = current.gamma;

				// Calculate the fbe use a proximal step, keep the proximal step in the cache
				// as we can reuse it if this step is accepted.
				prox_calc_.Calculate(potential_new_location, potential_new_prox_location, prox_calc_config_);
				auto new_fbe = fbe_.Eval(potential_new_location, potential_new_prox_location);

				if (new_fbe < fbe)
				{
					// Accept potential new location/prox_location
					const auto res = ResidualInfNorm(potential_new_prox_location);
					std::swap(potential_new_location, current); 
					std::swap(potential_new_prox_location, proximal); 
					return { res,new_fbe };
				}
			}

			// use only proximal gradient, no accelerator
			auto prox_config = decltype(prox_calc_)::default_config;
			std::swap(current, proximal); // Take a pure prox step.
			prox_calc_.Calculate(current,proximal, prox_config);
			const double res = ResidualInfNorm(proximal);
			const double new_fbe = fbe_.Eval(current, proximal);
			return { res, new_fbe };
		}
	};
}
