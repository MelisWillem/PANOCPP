#include <cassert>
#include <iostream>
#include <string>

// simple vector algebra
// two operations(applied element wise) are defined:
//    - v = v1 * v2
//    - v = v1 + v2
//
// extra:
// SUM -> calculates sum of all elements on the vector
namespace pnc {

template <unsigned int TSize, typename TData = double>
class VectorUnit {
private:
    const TData amplitude;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    VectorUnit(const TData amplitude)
        : amplitude(amplitude)
    {
    }

    TData operator[](unsigned int index) const
    {
        return amplitude;
    }
};

template <unsigned int TSize, typename TData = double>
class Vector {
private:
    TData* data;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    Vector()
        : data(new TData[size] {})
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

    TData operator[](unsigned int index) const
    {
        return data[index];
    }

    TData& operator[](unsigned int index)
    {
        return data[index];
    }

    Vector(Vector&& other)
        : data(other.data)
    {
        std::cout << "move called!";
    }

    template <typename TVecLike>
    Vector<size, TData>& operator=(const TVecLike& other)
    {
        for (unsigned int i = 0; i < size; i++) {
            data[i] = other[i];
        }

        return *this;
    }
};

template <typename TVec>
auto SUM(const TVec& vec)
{
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        cache += vec[i];
    }
    return cache;
}

template <typename TVec>
auto MAX(const TVec& vec)
{
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        if (cache < vec[i]) {
            cache = vec[i];
        }
    }
    return cache;
}

template <typename TVec>
auto MAX(const TVec left, const TVec right)
{
    return VectorMax(left, right);
}

template <typename TVector>
class VectorNegative {
private:
    const TVector& v;

public:
    static constexpr unsigned int size = TVector::size;
    using data_type=decltype(v[0]);

    VectorNegative(const TVector& v) : v(v) {}

    auto operator[](unsigned int index) const
    {
        return -v[index];
    }
};

template <typename TLeft, typename TRight>
class VectorSum {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorSum(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    auto operator[](unsigned int index) const
    {
        return v1[index] + v2[index];
    }
};

template <typename TLeft, typename TRight>
class VectorProd {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorProd(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    auto operator[](unsigned int index) const
    {
        return v1[index] * v2[index];
    }
};

template <typename TLeft, typename TRight>
class VectorMax {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorMax(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    auto operator[](unsigned int index) const
    {
        return v1[index] < v2[index]
            ? v2[index]
            : v1[index];
    }
};

template<typename TVec>
VectorNegative<TVec> operator-(const TVec& vec)
{
    return VectorNegative<TVec>(vec);
}

template <
    typename TLeft,
    typename TRight>
auto operator+(
    const TLeft& left,
    const TRight& right)
{
    return VectorSum<TLeft, TRight>(left, right);
}

template <
    typename TLeft,
    typename TRight>
auto operator*(
    const TLeft& left,
    const TRight& right)
{
    return VectorProd<TLeft, TRight>(left, right);
}

class ToVector {
};

template <typename TLeft>
auto operator|(
    const TLeft& left,
    const ToVector& right)
{
    auto vec = Vector<left.size, decltype(left[0])>();
    for (unsigned int i = 0; i < left.size; i++) {
        vec[i] = 1;
    }

    return vec;
}

}
