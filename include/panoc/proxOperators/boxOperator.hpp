#include <panoc/VectorAlgebra.hpp>
#include <type_traits>

namespace pnc {

// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of
// the closest border is returned.
// precondition: low < high
template <
    typename data_type_,
    typename TVector>
class BoxOperator final {
public:
    static constexpr unsigned int size = std::remove_reference_t<TVector>::size;
    using data_type = data_type_;

private:
    const TVector input;
    const data_type penalty = 1; // todo make this dyamic...
    const data_type low;
    const data_type high;
    data_type _cost;

public:
    explicit BoxOperator(
        TVector input,
        const data_type low,
        const data_type high)
        : input(std::forward<TVector>(input))
        , low(low)
        , high(high)
    {
        _cost = 0;
        for (std::size_t i = 0; i < input.size; ++i) {
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
};

// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of
// the closest border is returned.
// precondition: data_type == TVector::data_type
template <
    typename data_type,
    typename TVector,
    typename = typename std::enable_if_t<
        std::is_same_v<data_type, typename TVector::data_type>>>
constexpr auto Box(const TVector& input, const data_type low, const data_type high)
{
    // Sadly enough data_type needs to be defined by the user,
    // there must be a way to get rid of this !
    return BoxOperator<data_type, decltype(input)>(input, low, high);
}

template <
    typename data_type,
    typename TVector,
    typename = typename std::enable_if_t<
        std::is_same_v<data_type, typename TVector::data_type>>>
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

    template <
        typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename = std::enable_if_t<std::is_same_v<typename TVec::data_type, data_type>>>
    auto operator()(TVecRef&& input) const
    {
        return BoxOperator<data_type, TVecRef>
            (std::forward<TVecRef>(input), low, high);
    }
};

}
