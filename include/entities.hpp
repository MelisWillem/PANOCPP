#include<variant>
namespace pnc {

template <typename TVector, typename TConstant>
class Location {
public:
    Location(TVector location, TVector gradient, TConstant cost)
    {
        this->location = location;
        this->gradient = gradient;
        this->cost = cost;
    }

    const TVector location;
    const TVector gradient;
    const TConstant cost;
};

template <typename TVector, typename TConstant>
class ProxLocation : Location{
    ProxLocation (TVector location, TVector gradient, TConstant cost,TConstant gamma) :
        base(location,gradient,cost)
    {
        this->gamma=gamma;
    }
    const TConstant gamme;
};

// A location is either
// Location: not resulting from a proximal gradient step
// ProxLocation: the result of a proximal gradient, potentially with accelerator
using PncLocation<TVector,TConstant> = std::variant<
    Location<TVector,TConstant>,
    ProxLocation<TVector,TConstant>>;
}
