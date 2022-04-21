#pragma once
#include<type_traits>

namespace pnc{
	template<typename T>
	struct data_type;

	template<typename T>
	struct index_type;

	template<typename T>
	using data_type_t = typename data_type<T>::type;

	template<typename T>
	using index_type_t = typename index_type<T>::type;

	template<typename, typename=void, typename=void>
	struct is_vector_type : std::false_type{};

	template<typename TVec>
	struct is_vector_type<
		TVec, 
		data_type_t<TVec>,
		index_type_t<TVec>> : std::true_type
	{
	};

	template<typename TVec>
	constexpr bool is_vector_type_v = is_vector_type<TVec>::value;
}
