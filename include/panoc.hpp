#include"vectorAlgebra.hpp"
#include"ProximalCalculator.hpp"
#include"LBFGS.hpp"
#include<math.h>

namespace pnc {
	template<typename TCostFunc, typename TProx>
	class Panoc {
		struct Config {
			int max_iterations;
			int max_fbe_iterations;
			int min_residual;
		};

		TCostFunc& cost_function_;
		TProx& prox_;
		Config config_;
		pnc::ProximalCalculator<TCostFunc, TProx> prox_calc_;
		typename pnc::ProximalCalculator<TCostFunc, TProx>::Config prox_calc_config_ = pnc::ProximalCalculator<TCostFunc, TProx>::default_config;
		pnc::FBE<TCostFunc, TProx> fbe_;
		ProximalGradientStep<Vector<2, double>> cache;
		LBFGS<10, double, 2> accelerator_;
		
	public:
		Panoc(TCostFunc cost_function,TProx& prox, Config& config )
			: cost_function_(cost_function),
			prox_(prox),
			config_(config),
			prox_calc_(cost_function_,prox_),
			fbe_(cost_function, prox_),
			accelerator_()
		{
				
		}

		template<typename TVec>
		void Solve(TVec& vector)
		{
           for (int i = 0; i < config_.max_iterations && residual>min_residual; ++i)
            {
            //    var oldProx = prox;

            //    var oldGamma = prox.ProxLocation.Gamma;
            //    if (accelerator.HasCache) // If there is accelstep(which needs previous runs) then we can improve stuff
            //    {
            //        (residual, prox, fbe,tau) = Search(prox, fbe,function,proxFunction,config,accelerator.GetStep(prox.Location));
            //    }
            //    else 
            //    {
            //        prox = ProximalGradientStep.Calculate(prox.ProxLocation,config,function,proxFunction);
            //        fbe = ForwardBackwardEnvelop.Calculate(prox);
            //    }

            // This update doesn't always mean that the cache will be updated,
            // the lbfgs does a carefull update and will refuse some updates due to beein badly conditioned
            //    if (oldGamma != prox.ProxLocation.Gamma) { accelerator.Reset(); }
            //    var cacheUpdated = accelerator.Update(oldLocation: oldProx.Location, newLocation: prox.Location);
            }
		}

		template<typename TVec>
		double ResidualInfNorm(TVec vec)
		{
			return 0;
		}

		// return {residual, fbe, tau}
		template<typename TVec>
		std::tuple<double, double, double, ProximalGradientStep<TVec>> Search(ProximalGradientStep<TVec>&& prox_step, double fbe, TVec& acceleration_step)
		{
			auto tau = [](auto i) {return pow(2, i); }
			// linesearch on fbe, if the condition is never met. Use
			// as pure proximal gradient step.
			for(int i = 0; i < config_.max_fbe_iterations; ++i)
			{
				// Take a step away from the current location, using part proximal
				// and part the accerator.
				cache.current.location = prox_step.current.location 
					+ (prox_step.proximal.position - prox_step.current.position)*(1-tau(i))
					+ acceleration_step * tau(i);
				cache.cost = cost_function_(cache.current.location, cache.current.gradient);
				cache.gamma = prox_step.gamma;
				
				// Calculate the fbe use a proximal step, keep the proximal step in the cache
				// as we can reuse it if this step is accepted.
				cache = prox_calc_.Calculate(std::move(cache), prox_calc_config_);
				auto new_fbe = fbe.Eval(cache);

                if (new_fbe< fbe)
                {
					const auto res = ResidualInfNorm(cache.proximal.location);
					std::swap(cache, prox_step);
                    return {res,new_fbe,tau(i),std::move(prox_step)};
                }
			}

            // use only proximal gradient, no accelerator
            auto pureProx = prox_calc_.Calculate(prox.ProxLocation,config,function,proxFunc);
			const double tau = 0;
			const double res = Residual(pure_prox);
			return {res,pureProx,fbe.Eval(new_prox),0, std::move(prox_step) };
		}
	};
}
