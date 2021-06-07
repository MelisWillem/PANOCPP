#pragma once

#include <initializer_list>
#include <array>

namespace pnc {
	template <unsigned int TSize, typename TData = double>
	class Vector final {
	private:
		std::array<TData, TSize> data;

	public:
		static constexpr unsigned int size = TSize;
		using data_type = TData;

		Vector() : data(){}

		Vector(std::initializer_list<TData> input)
		{
			size_t i = 0;
			for (const auto val : input)
			{
				data[i] = val;
				++i;
			}
		}

		Vector(const Vector<size, data_type>& other)
		{
			for (int i = 0; i < size; ++i)
			{
				data[i] = other[i];
			}
		}

		constexpr TData operator[](unsigned int index) const
		{
			return data[index];
		}

		constexpr TData& operator[](unsigned int index)
		{
			return data[index];
		}

		Vector(Vector&& other)
			: data(std::move(other.data))
		{
		}

		template <
			typename TVecRef,
			typename TVec = std::remove_reference_t<TVecRef>>
			constexpr Vector<size, TData>& operator=(TVecRef&& other)
		{
			static_assert(
				size == TVec::size,
				"Trying to assign vector expression to a vector of a different dimension");
			for (unsigned int i = 0; i < size; i++) {
				data[i] = other[i];
			}

			return *this;
		}
	};

}
