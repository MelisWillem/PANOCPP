#include<panoc/VectorAlgebra.hpp>
#include<panoc/proxOperators.hpp>
#include<panoc/ProximalCalculator.hpp>
#include<panoc/LBFGS.hpp>
#include<panoc/fbe.hpp>
#include<panoc/LocationBuilder.hpp>
#include<math.h>

namespace pnc {
	struct PanocConfig {
		int max_iterations;
		int max_fbe_iterations;
		double min_residual;
		int lbfgs_cache_size;
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
		LBFGS<double, int> accelerator_;

	public:
		// TODO:: make in generic
		Panoc(TCostFunc& cost_function, TProx& prox, PanocConfig& config, int dimension)
			: cost_function_(cost_function),
			prox_(prox),
			config_(config),
			prox_calc_(cost_function_, prox_),
			fbe_(cost_function, prox_),
			accelerator_(dimension, config.lbfgs_cache_size)
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
