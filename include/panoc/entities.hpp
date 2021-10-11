#pragma once

#include<utility>

namespace pnc {

	template <
		typename TVectorRef,
		typename TVector = std::remove_reference_t<TVectorRef>,
		typename TConstant = typename TVector::data_type,
		typename TSize = typename TVector::size_type
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

		Location(TSize dimension) :
			location(dimension),
			gradient(dimension),
			gamma(0),
			cost(0)
		{
		}

		template<typename TVec>
		Location(Location<TVec>&& other) 
			: location(std::move(other.location)),
			gradient(std::move(other.gradient)),
			cost(cost),
			gamma(gamma){}

		template<typename TVec>
		Location(const Location<TVec>& other)
			:
			location(other.location),
			gradient(other.gradient),
			gamma(other.gamma),
			cost(other.cost)
		{
		}

		//template<typename TVec>
		//Location<TVec>& operator=(const Location<TVec>& other)
		//{
		//	location = other.location;
		//	gradient = other.gradient;
		//	gamma = other.gamma;
		//	 cost = other.cost;

		//	return this;
		//}

		TVectorRef location;
		TVectorRef gradient;
		TConstant gamma;
		TConstant cost;
	};

}
