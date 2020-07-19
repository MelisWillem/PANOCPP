#include<variant>
namespace pnc {

template <
    typename TVector,
    typename TConstant = typename TVector::data_type
    >
class Location {
public:
    Location(TVector&& location, TVector&& gradient,TConstant cost ): 
        location(std::forward(location)),
        gradient(std::forward(gradient)),
        cost(cost)
    {
        this->location = location;
        this->gradient = gradient;
        this->cost = cost;
    }

    const TVector location;
    const TVector gradient;
    const TConstant cost;
};

template <
    typename TVector,
    typename TConstant = typename TVector::data_type
    >
class ProxLocation : public Location<TVector>{
    ProxLocation (TVector&& location, TVector&& gradient, TConstant cost,TConstant gamma) :
        Location<TVector>(   std::forward(location),
                std::forward(gradient),
                cost)
    {
        this->gamma=gamma;
    }
    const TConstant gamme;
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

