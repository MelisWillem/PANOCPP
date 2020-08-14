#pragma once

namespace pnc{
    template <unsigned int TSize, typename TData = double>
    class Vector final{
    private:
        TData* data;
        bool is_moved = false;

    public:
        static constexpr unsigned int size = TSize;
        using data_type = TData;

        Vector()
            : data(new TData[size] {})
        {
        }

        template<
            typename ...TArgs,
            typename = typename std::enable_if_t<size==sizeof...(TArgs)>
                >
        Vector(TArgs... args)
            : data(new TData[TSize]{args...})
        {
        }

        Vector(TData init[size])
            : data(init)
        {
        }

        ~Vector()
        {
            delete[] data;
        }

        constexpr TData operator[](unsigned int index) const
        {
            assert(!IsMoved());
            return data[index];
        }

        constexpr TData& operator[](unsigned int index)
        {
            assert(!IsMoved());
            return data[index];
        }

        Vector(Vector&& other)
            : data(other.data)
        {
            other.data = {};
            other.is_moved = true;
        }

        template <
            typename TVecRef,
            typename TVec = std::remove_reference_t<TVecRef>
            >
        constexpr Vector<size, TData>& operator=(TVecRef&& other)
        {
            static_assert(
                size == TVec::size,
                "Trying to assign vector expression to a vector of a different dimension");
            assert(!IsMoved());
            for (unsigned int i = 0; i < size; i++) {
                data[i] = other[i];
            }

            return *this;
        }

        bool IsMoved() const 
        {
            return is_moved;
        }
    };

}
