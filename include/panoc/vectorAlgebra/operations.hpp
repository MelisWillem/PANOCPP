#pragma once

namespace pnc{

    template <
        typename TLeftRef,
        typename TRightRef,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename TRight = std::remove_reference_t<TRightRef>,
        unsigned int left_size = TLeft::size,
        unsigned int right_size = TRight::size,
        typename = typename std::enable_if_t<left_size==right_size>
        >
    constexpr auto ComponentWiseMax(
        TLeftRef&& left,
        TRightRef&& right)
    {
        return VectorComponentMax<TLeftRef, TRightRef>(
                std::forward<TLeftRef>(left),
                std::forward<TRightRef>(right));
    }

    template <
        typename TVector,
        typename data_type = typename TVector::data_type
        >
    constexpr data_type Norm(TVector& v)
    {
        return (v * v);
    }

    template <
        typename TVector,
        typename TSQRT,
        typename data_type = typename TVector::data_type
        >
    constexpr auto Norm2(TVector& v,TSQRT sqrtOperator)
    {
        return sqrtOperator(v * v);
    }

    template<
        typename TVectorRef,
        typename TVector = std::remove_reference_t<TVectorRef>,
        unsigned int size = TVector::size,
        typename data_type = typename TVector::data_type
        >
    constexpr auto NormL1(TVectorRef&& vec) 
    {
        return SUM(VectorAbs<TVectorRef>(std::forward<TVectorRef>(vec)));
    }
}
