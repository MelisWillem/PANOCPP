#pragma once

#include <vector>

namespace pnc{
    template<typename TMatrix>
	class CollumOfMatrixProxy{
	public:
		using data_type = typename TMatrix::data_type;
		using size_type = typename TMatrix::size_type;

	private:
		TMatrix& m;
		size_type col_id;

	public:
		CollumOfMatrixProxy(TMatrix& m_, size_type col_id_) 
			: m(m_), col_id(col_id_)
		{}

		auto& operator[](size_type id)
		{
			return m.at(id, col_id);
		}

		auto operator[](size_type id) const
		{
			return m.at(id, col_id);
		}

		auto size() const
		{
			return m.dimensions().first;
		}

		template <
			typename TVecRef,
			typename TVec = std::remove_reference_t<TVecRef>,
			typename TDataTVec = typename TVec::data_type,
			typename TSize = typename TVec::size_type
		>
		auto& operator=(TVecRef&& other)
		{
			assert(size() == other.size());
			for (TSize i = 0; i < size(); i++) {
				m.at(i, col_id) = other[i];
			}

			return *this;
		}
	};

    template<typename TData = double, typename TSize = int>
    class Matrix final{
	public:
		using data_type = TData;
		using size_type = TSize;
	private:
		// collum wise matrix, aka Fortran style
		std::vector<TData> data;
		size_type numOfRows;
		size_type numOfCols;
	public:
		Matrix(size_type numOfCols_, size_type numOfRows_)
			: numOfCols(numOfCols_),
			  numOfRows(numOfRows_)
		{
			data.resize(numOfCols_*numOfRows_);
		}

		auto size() const
		{
			return (size_type)data.size();
		}

		// returns {numOfRows, numOfCols}
		std::pair<size_type, size_type> dimensions() const 
		{
			return {numOfRows, numOfCols};
		}

		auto operator[](size_type col_id)
		{
			return CollumOfMatrixProxy(*this, col_id);
		}

		auto& at(size_type row_id, size_type col_id) 
		{
			return data[col_id*numOfRows + row_id];
		}

		auto at(size_type row_id, size_type col_id) const 
		{
			return data[col_id*numOfRows + row_id];
		}
    };
}
