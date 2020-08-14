#pragma once

namespace pnc{
    template <
        typename TVecRef ,
        typename TVec = std::remove_reference_t<TVecRef>,
        unsigned int size = TVec::size
        >
    constexpr auto SUM(TVecRef&& vec)
    {
        static_assert(size> 0,"Sum of a vector is only defined if the dimension of the vector is larger then 0");
        auto cache = vec[0];
        for (unsigned int i = 1; i < vec.size; i++) {
            cache += vec[i];
        }
        return cache;
    }
}
