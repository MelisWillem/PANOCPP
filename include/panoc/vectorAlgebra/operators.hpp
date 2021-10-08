#pragma once

#include<type_traits>
#include<utility>
#include<panoc/vectorAlgebra/vectorMap.hpp>
#include<panoc/vectorAlgebra/templateHelpers.hpp>
#include<panoc/vectorAlgebra/vectorUnit.hpp>
#include<panoc/vectorAlgebra/vectorPairwiseMap.hpp>

namespace pnc{

    template <
        typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename = typename TVec::data_type,
        typename = typename TVec::size_type
            >
    constexpr auto operator-(TVecRef&& vec)
    {
        return VectorNegative<TVecRef>(std::forward<TVecRef>(vec));
    }

    // vector + constant
    template <
        typename TLeftRef,
        typename TRight,
        typename TLeft = typename std::remove_reference_t<TLeftRef>,
        typename = typename TLeft::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
    constexpr auto operator+(
        TLeftRef&& left,
        TRight right)
    {
        using TRightVec = VectorUnit<typename TLeft::size_type, typename TLeft::data_type>;
        return VectorSum<TLeftRef, TRightVec>
            (std::forward<TLeftRef>(left), TRightVec(right, left.size()));
    }

    // constant + vector
    template <
        typename TLeft,
        typename TRightRef,
        typename TRight = typename std::remove_reference_t<TRightRef>,
        typename = typename TRight::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
    constexpr auto operator+(
        TLeft left, // not a vector
        TRightRef&& right)
    {
        using TLeftVector = VectorUnit<typename TRight::size_type, typename TRight::data_type>;
        return VectorSum<TLeftVector,TRightRef>
            (TLeftVector(left, right.size()),std::forward<TRightRef>(right));
    }

    // vector * constant
    template <
        typename TLeftRef,
        typename TRight,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename = typename TLeft::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
    auto operator*(
        TLeftRef&& left,
        TRight right)
    {
        using TRightVec = VectorUnit<typename TLeft::size_type, typename TLeft::data_type>;
        return VectorProd<TLeftRef,TRightVec>(std::forward<TLeftRef>(left),TRightVec(right, left.size()));
    }

    // constant * vector
    template <
        typename TLeft,
        typename TRightRef,
        typename TRight = std::remove_reference_t<TRightRef>,
        typename = typename TRight::data_type,
        typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
    constexpr auto operator*(
        TLeft left,
        TRightRef&& right)
    {
        using TLeftVector = VectorUnit<typename TRight::size_type,typename TRight::data_type>;
        return VectorProd<TLeftVector,TRightRef>(TLeftVector(left,right.size()),std::forward<TRightRef>(right));
    }

    template <
        typename TLeftRef,
        typename TRightRef,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename TRight = std::remove_reference_t<TRightRef>,
        typename typeL = typename TLeft::data_type,
        typename typeR = typename TRight::data_type
            >
    constexpr auto operator-(
        TLeftRef&& left,
        TRightRef&& right)
    {
        return left + (-right);
    }

    template <
        typename TLeftRef,
        typename TRightRef,
        typename TRight = std::remove_reference_t<TRightRef>,
        typename TLeft = std::remove_reference_t<TLeftRef>,
        typename typeL = typename TLeft::data_type,
        typename typeR = typename TRight::data_type,
        typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>
        >
    constexpr auto operator+(
        TLeftRef&& left,
        TRightRef&& right)
    {
        assert(left.size() == right.size());
        return VectorSum<TLeftRef,TRightRef>(
            std::forward<TLeftRef>(left),
            std::forward<TRightRef>(right));
    }

    template <
        typename TLeftRef,
        typename TRightRef,
        typename TLeft = typename std::remove_reference_t<TLeftRef>,
        typename TRight = typename std::remove_reference_t<TRightRef>,
        typename typeL = typename TLeft::data_type,
        typename typeR = typename TRight::data_type,
        typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>
        >
    constexpr auto operator*(
        TLeftRef&& left,
        TRightRef&& right)
    {
        return SUM(
            VectorProd<TLeftRef, TRightRef>(
                std::forward<TLeftRef>(left),
                std::forward<TRightRef>(right)));
    }
}
