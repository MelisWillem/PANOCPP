#pragma once

namespace pnc{

template<typename TData>
struct MaxOperation{
    constexpr auto eval(TData x,TData y) const
    {
        return std::max(x,y);
    }
};

template<typename TData>
struct MulOperation{
    constexpr auto eval(TData x,TData y) const
    {
        return x * y;
    }
};

template<typename TData>
struct AddOperation{
    constexpr auto eval(TData x,TData y) const
    {
        return x + y;
    }
};

template< typename data_type >
struct AbsOp{
     constexpr auto eval(data_type x) const
    {
        return abs(x);
    }
};

template<typename data_type>
struct Negative{
    constexpr auto eval(data_type x)  const
    {
        return -x;
    }
};
}
