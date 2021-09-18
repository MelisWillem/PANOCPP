#pragma once

#include<limits>

namespace pnc {
	template<
		typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
		typename TData = typename TVec::data_type,
        typename TSize = typename TVec::size_type
	>
		TData InfNorm(TVecRef&& vec)
	{
		TData max = std::numeric_limits<TData>::min();

		for (TSize i = 0; i < vec.size(); ++i)
		{
			if (max < vec[i]) { max = vec[i]; }
		}

		return max;
	}
}