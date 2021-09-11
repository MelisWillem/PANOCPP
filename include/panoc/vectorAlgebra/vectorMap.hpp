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
        static constexpr unsigned int size = TVec::size;
        using data_type = typename TVec::data_type;

        VectorMap(TVecRef&& v) 
            : v(std::forward<TVecRef>(v)) {}

        constexpr auto operator[](unsigned int index) const
        {
            return TFunc<data_type>().eval(v[index]);
        }
    };

    template<typename TVector>
    using VectorNegative = VectorMap<TVector,Negative>;

    template<typename TVector>
    using VectorAbs = VectorMap<TVector,AbsOp>;
}
