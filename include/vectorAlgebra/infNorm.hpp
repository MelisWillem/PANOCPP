#pragma once

#include<limits>

namespace pnc {
	template<
		typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
		typename TData = typename TVec::data_type,
        unsigned int size = TVec::size
	>
		TData InfNorm(TVecRef&& vec)
	{
		TData max = std::numeric_limits<TData>::min();

		for (int i = 0; i < size; ++i)
		{
			if (max < vec[i]) { max = vec[i]; }
		}

		return max;
	}
}