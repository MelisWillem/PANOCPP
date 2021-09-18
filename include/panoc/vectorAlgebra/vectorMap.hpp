#pragma once

namespace pnc{
    template <
        typename TVecRef,
        template<typename> typename TFunc,
        typename TVec = typename std::remove_reference_t<TVecRef>
        >
    class VectorMap final {
    private:
        TVecRef v;

    public:
        using data_type = typename TVec::data_type;
        using size_type = typename TVec::size_type;

        VectorMap(TVecRef&& v) 
            : v(std::forward<TVecRef>(v)) {}

        size_type size() const {
            return v.size();
        }

        constexpr auto operator[](size_type index) const
        {
            return TFunc<data_type>().eval(v[index]);
        }
    };

    template<typename TVector>
    using VectorNegative = VectorMap<TVector,Negative>;

    template<typename TVector>
    using VectorAbs = VectorMap<TVector,AbsOp>;
}
