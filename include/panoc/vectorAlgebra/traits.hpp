#pragma once
namespace pnc{

template<typename TVector>
struct VectorTraits;

template<typename TData,auto TSize>
struct VectorTraits<pnc::Vector<TSize, TData>>
{
    using DataType = TData;
    static constexpr auto size = TSize;
};

template<typename TVec, template<typename> typename TPolicy>
struct VectorTraits<pnc::VectorMap<TVec, TPolicy>>{
    using DataType = typename VectorTraits<TVec>::DataType;
    static constexpr auto size = VectorTraits<TVec>::size;
};

template<auto TSize, typename TData>
struct VectorTraits<pnc::VectorUnit<TSize, TData>>{
    using DataType = TData;
    static constexpr auto size = TSize;
};

template<
    typename TVecLeft,
    typename TVecRight,
    template<typename> typename TPairPolicy
>
struct VectorTraits<VectorPairwiseMap<TVecLeft, TVecRight, TPairPolicy>>{
    // We should also make sure that the left and right vector
    // have the same size and datatype when this trait is called.
    using DataType = typename VectorTraits<TVecLeft>::DataType;
    static constexpr auto size = VectorTraits<TVecLeft>::size;
};

}
