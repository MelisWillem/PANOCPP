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

			auto c1 = LocationBuilder<TVec>().Build(
				cost_function_,
				std::move(vector),
				std::move(gradient),
				cost,
				cache
			);
			Location<TVec> c2;
			ProximalGradientStep<TVec> current_prox_step = { c1, c2 };

			auto prox_config = decltype(prox_calc_)::default_config;
			auto prox_step = prox_calc_.Calculate(std::move(current_prox_step), prox_config);

			for (int i = 0; i < config_.max_iterations && residual>config_.min_residual; ++i)
			{
				Location<TVec> old_l = prox_step.current;
				Location<TVec> old_r = prox_step.proximal;
				ProximalGradientStep<TVec> old_prox = { old_l, old_r };

				auto oldGamma = prox_step.proximal.gamma;
				if (accelerator_.hasCache()) // If there is accelstep(which needs previous runs) then we can improve stuff
				{
					TVec accelerator_step;
					accelerator_.solve(prox_step.current.gradient, accelerator_step);
					auto [residual_, fbe_, prox_step_] = Search(std::move(prox_step), fbe, accelerator_step);
				}
				else
				{
					// prox_step = prox_calc_.Calculate(std::move(prox_step), decltype(prox_calc_)::default_config);
					// fbe = fbe_.Eval(current);
				}

				// This update doesn't always mean that the cache will be updated,
				// the lbfgs does a carefull update and will refuse some updates due to beein badly conditioned
				if (oldGamma != prox_step.proximal.gamma) { accelerator_.Reset(); }
				bool cache_updated = accelerator_.updateHessian(
					old_prox.current.location,
					old_prox.current.gradient,
					prox_step.proximal.location,
					prox_step.proximal.gradient);
			}
		}

		template<typename TVec>
		double ResidualInfNorm(TVec vec)
		{
			return 0;
		}

		// return {residual, fbe, step}
		template<typename TVec>
		std::tuple<double, double, ProximalGradientStep<TVec>> Search(
			ProximalGradientStep<TVec>&& prox_step,
			double fbe,
			TVec& acceleration_step)
		{
			auto tau = [](auto i) {return pow(2, i); };
			// linesearch on fbe, if the condition is never met. Use
			// as pure proximal gradient step.
			for (int i = 0; i < config_.max_fbe_iterations; ++i)
			{
				Location<TVec> c1;
				Location<TVec> c2;
				ProximalGradientStep<TVec> cache{ std::move(c1),std::move(c2) };
				// Take a step away from the current location, using part proximal
				// and part the accerator.
				cache.current.location = prox_step.current.location
					+ (prox_step.proximal.location - prox_step.current.location) * (1 - tau(i))
					+ acceleration_step * tau(i);
				cache.current.cost = cost_function_(cache.current.location, cache.current.gradient);
				cache.current.gamma = prox_step.current.gamma;

				// Calculate the fbe use a proximal step, keep the proximal step in the cache
				// as we can reuse it if this step is accepted.
				// cache = prox_calc_.Calculate(std::move(cache), prox_calc_config_);
				auto new_fbe = fbe_.Eval(cache);

				if (new_fbe < fbe)
				{
					const auto res = ResidualInfNorm(cache.proximal.location);
					//std::swap(cache, prox_step);
					return { res,new_fbe,std::move(prox_step) };
				}
			}

			// use only proximal gradient, no accelerator
			//auto prox_config = decltype(prox_calc_)::default_config;
			//auto pure_prox = prox_calc_.Calculate(std::move(prox_step), prox_config);
			//const double res = ResidualInfNorm(pure_prox);
			// return { res,fbe_.Eval(pure_prox), std::move(prox_step) };
		}
	};
}
