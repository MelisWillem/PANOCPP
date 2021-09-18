#pragma once

namespace pnc{
    template <
        typename TVecRef ,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename TSize = typename TVec::size_type
        >
    constexpr auto SUM(TVecRef&& vec)
    {
        assert(vec.size()> 0);
        auto cache = vec[0];
        for (TSize i = 1; i < vec.size(); i++) {
            cache += vec[i];
        }
        return cache;
    }
}
