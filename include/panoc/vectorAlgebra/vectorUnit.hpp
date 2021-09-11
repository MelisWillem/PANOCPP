#pragma once

namespace pnc{

template <unsigned int TSize, typename TData = double>
class VectorUnit final {
private:
    TData constant;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    explicit VectorUnit(TData constant): constant(constant)
    {}

    constexpr auto operator[](unsigned int index) const
    {
        return constant;
    }
};

}
