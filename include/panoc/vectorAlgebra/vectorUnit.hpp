#pragma once

namespace pnc{

template <typename TSize, typename TData = double>
class VectorUnit final {
private:
    TData constant;
    TSize size_vector;

public:
    TSize size() const {
        return size_vector;
    }
    using data_type = TData;
    using size_type = TSize;

    explicit VectorUnit(TData constant, TSize s): constant(constant), size_vector(s)
    {}

    constexpr auto operator[](unsigned int index) const
    {
        return constant;
    }
};

}
