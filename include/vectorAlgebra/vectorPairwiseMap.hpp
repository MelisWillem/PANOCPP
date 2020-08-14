#pragma once

namespace pnc{
template <
    typename TLeftRef,
    typename TRightRef,
    template<typename> typename PairOp
        >
class VectorPairwiseMap final{
private:
    TLeftRef v1;
    TRightRef v2;

public:
    using TLeft = typename std::remove_reference_t<TLeftRef>;
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    explicit VectorPairwiseMap(
        TLeftRef&& left,
        TRightRef&& right)
        : v1(std::forward<TLeftRef>(left))
        , v2(std::forward<TRightRef>(right))
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return PairOp<data_type>().eval(v1[index],v2[index]);
    }
};

template< typename TLeftRef, typename TRightRef >
using VectorSum = VectorPairwiseMap<TLeftRef,TRightRef,AddOperation>;

template< typename TLeftRef, typename TRightRef >
using VectorProd = VectorPairwiseMap<TLeftRef,TRightRef,MulOperation>;

template< typename TLeftRef, typename TRightRef >
using VectorComponentMax = VectorPairwiseMap<TLeftRef,TRightRef,MaxOperation>;
}

