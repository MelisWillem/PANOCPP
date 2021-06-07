#pragma once

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
