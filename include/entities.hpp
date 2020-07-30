#include<utility>
#include<variant>

namespace pnc {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <
    typename TVector,
    typename TConstant = typename TVector::data_type
    >
class Location {
public:
    Location(TVector&& location, TVector&& gradient,TConstant cost ): 
        location(std::forward<TVector>(location)),
        gradient(std::forward<TVector>(gradient)),
        cost(cost)
    {
    }

    TVector location;
    TVector gradient;
    TConstant cost;
};

template <
    typename TVector,
    typename TConstant = typename TVector::data_type
    >
class ProxLocation : public Location<TVector>{
    ProxLocation (TVector&& location, TVector&& gradient, TConstant cost,TConstant gamma) :
        Location<TVector>(std::forward(location),
                std::forward(gradient),
                cost),gamma(gamma)
    {
    }
    TConstant gamma;
};

// A location is either
// Location: not resulting from a proximal gradient step
// ProxLocation: the result of a proximal gradient, potentially with accelerator
template<
    typename TVector,
    typename TConstant = typename TVector::data_type
    >
using PncLocation = std::variant<
    Location<TVector>,
    ProxLocation<TVector>>;
}

