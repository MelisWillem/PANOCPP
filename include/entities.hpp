#pragma once

#include<utility>

namespace pnc {

	template <
		typename TVectorRef,
		typename TVector = std::remove_reference_t<TVectorRef>,
		typename TConstant = typename TVector::data_type
	>
		struct Location {
		Location(
			TVectorRef&& location,
			TVectorRef&& gradient,
			TConstant cost,
			TConstant gamma) :
			location(std::forward<TVectorRef>(location)),
			gradient(std::forward<TVectorRef>(gradient)),
			gamma(gamma),
			cost(cost)
		{ }

		Location() :
			location(),
			gradient(),
			gamma(0),
			cost(0)
		{
		}

		template<typename TVec>
		Location(const Location<TVec>& other)
			:
			location(other.location),
			gradient(other.gradient),
			gamma(other.gamma),
			cost(other.cost)
		{
		}

		TVectorRef location;
		TVectorRef gradient;
		TConstant gamma;
		TConstant cost;
	};

}
