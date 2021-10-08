#pragma once

#include <type_traits>
#include <utility>

namespace pnc {

// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of
// the closest border is returned.
// precondition: low < high
template <
    typename TVectorRef,
    typename TVector = typename std::remove_reference_t<TVectorRef>,
    typename TSize = typename TVector::size_type,
    typename TData = typename TVector::data_type
>
class BoxOperator final {
public:
    using data_type = TData;
    using size_type = TSize;

private:
    const TVectorRef input;
    const data_type penalty = 1; // todo make this dyamic...
    const data_type low;
    const data_type high;
    data_type _cost;

public:
    explicit BoxOperator(
        TVectorRef&& input,
        const data_type low,
        const data_type high)
        : input(std::forward<TVectorRef>(input))
        , low(low)
        , high(high)
    {
        _cost = 0;
        for (size_type i = 0; i < input.size(); ++i) {
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

    auto size() const
    {
        return input.size();
    }
};

// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of
// the closest border is returned.
template<
    typename TVectorRef,
    typename TVector = std::remove_reference_t<TVectorRef>
>
constexpr auto Box(
    TVectorRef&& input,
    const typename TVector::data_type low,
    const typename TVector::data_type high)
{
    return BoxOperator<decltype(input)>
        (std::forward<TVectorRef>(input), low, high);
}

template <
    typename data_type
>
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

    template<
        typename TVecRef,
        typename TVec = typename std::remove_reference_t<TVecRef>,
        typename = typename std::enable_if_t<std::is_same<data_type, typename TVec::data_type>::value>
    >
    auto operator()(TVecRef&& input) const
    {
        return BoxOperator<TVecRef>
            (std::forward<TVecRef>(input), low, high);
    }
};

}
