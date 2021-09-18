#pragma once

namespace pnc{
    template <
        typename TVec,
        typename TSize = typename TVec::size_type
        >
    constexpr auto MAX(const TVec& vec)
    {
        assert(vec.size() > 0);
        auto cache = vec[0];
        for (TSize i = 1; i < vec.size(); i++) {
            if (cache < vec[i]) {
                cache = vec[i];
            }
        }
        return cache;
    }

}
