﻿#pragma once

#include <initializer_list>
#include <vector>

namespace pnc {
	template <typename TData = double>
	class Vector final {
	private:
		std::vector<TData> data;

	public:
		using data_type = TData;
		using size_type = int;

		Vector(unsigned int size) : data(size){}

		Vector(std::initializer_list<TData> input) : data(input)
		{
		}

		Vector(const Vector<data_type>& other)
		{
			for (int i = 0; i < size(); ++i)
			{
				data[i] = other[i];
			}
		}

		auto size() const
		{
			return (size_type)data.size();
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

		Vector<TData>& operator=(const Vector<TData>& other)
		{
			for (unsigned int i = 0; i < size(); i++) {
				data[i] = other[i];
			}

			return *this;
		}

		template <
			typename TVecRef,
			typename TVec = std::remove_reference_t<TVecRef>,
			typename TData = typename TVec::data_type,
			typename TSize = typename TVec::size_type
		>
		Vector<TData>& operator=(TVecRef&& other)
		{
			assert(size() == other.size());
			for (TSize i = 0; i < size(); i++) {
				data[i] = other[i];
			}

			return *this;
		}
	};
}
