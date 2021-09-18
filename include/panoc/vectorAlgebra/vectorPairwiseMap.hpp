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
    using data_type = typename TLeft::data_type;
    using size_type = typename TLeft::size_type;
    size_type vector_size;

    explicit VectorPairwiseMap(
        TLeftRef&& left,
        TRightRef&& right)
        : v1(std::forward<TLeftRef>(left))
        , v2(std::forward<TRightRef>(right)),
        vector_size(left.size())
    {
        assert(left.size() == right.size());
    }

    size_type size() const {
        return vector_size;
    }

    constexpr auto operator[](size_type index) const
    {
        return PairOp<data_type>().eval(v1[index],v2[index]);
    }
};

template< typename TLeftRef, typename TRightRef >
using VectorSum = VectorPairwiseMap<TLeftRef, TRightRef, AddOperation>;

template< typename TLeftRef, typename TRightRef >
using VectorProd = VectorPairwiseMap<TLeftRef,TRightRef,MulOperation>;

template< typename TLeftRef, typename TRightRef >
using VectorComponentMax = VectorPairwiseMap<TLeftRef,TRightRef,MaxOperation>;
}

