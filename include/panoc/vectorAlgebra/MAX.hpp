#pragma once

namespace pnc{
    template <
        typename TVec,
        unsigned int size = TVec::size
        >
    constexpr auto MAX(const TVec& vec)
    {
        static_assert(size> 0,"Max of a vector is only defined if the dimension of the vector is larger then 0");
        auto cache = vec[0];
        for (unsigned int i = 1; i < vec.size; i++) {
            if (cache < vec[i]) {
                cache = vec[i];
            }
        }
        return cache;
    }

}
