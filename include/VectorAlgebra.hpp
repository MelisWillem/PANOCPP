#pragma once
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <stdlib.h>

namespace pnc {
template <unsigned int TSize, typename TData = double>
class VectorUnit final {
private:
    TData constant;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    VectorUnit(TData constant): constant(constant)
    {}

    constexpr auto operator[](unsigned int index) const
    {
        return constant;
    }
};

template <unsigned int TSize, typename TData = double>
class Vector final{
private:
    TData* data;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    Vector()
        : data(new TData[size] {})
    {
    }

    template<typename ...TArgs,typename = typename std::enable_if_t<size==sizeof...(TArgs)>>
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
        return data[index];
    }

    constexpr TData& operator[](unsigned int index)
    {
        return data[index];
    }

    Vector(Vector&& other)
        : data(other.data)
    {
        other.data = {};
    }

    template <typename TVecLike>
    constexpr Vector<size, TData>& operator=(const TVecLike& other)
    {
        static_assert(
                size == TVecLike::size,
                "Trying to assign vector expression to a vector of a different dimension");
        for (unsigned int i = 0; i < size; i++) {
            data[i] = other[i];
        }

        return *this;
    }
};

template <typename TVector, typename TMaybeData>
struct IsDataOfVectorKind {
    static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
};

template <
    typename TVec,
    unsigned int size = TVec::size
    >
constexpr auto SUM(const TVec& vec)
{
    static_assert(size> 0,"Sum of a vector is only defined if the dimension of the vector is larger then 0");
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        cache += vec[i];
    }
    return cache;
}

template <typename TVec,unsigned int size = TVec::size>
constexpr auto MAX(const TVec& vec)
{
    static_assert(size> 0,"Max of a vector is only defined if the dimension of the vector is larger then 0");
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        if (cache < vec[i]) {
            cache = vec[i];
        }
    }
    return cache;
}

template <
    typename TLeft,
    typename TRight,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if_t<sizeL==sizeR>
    >
constexpr auto MAX(const TLeft left, const TRight right)
{
    return VectorMax(left, right);
}

template <typename TVector>
class VectorNegative final {
private:
    const TVector& v;

public:
    static constexpr unsigned int size = TVector::size;
    using data_type = decltype(v[0]);

    VectorNegative(const TVector& v)
        : v(v)
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return -v[index];
    }
};

template <typename TVector, typename TConstant,typename TOperator>
class VectorConstantOperation final{
private:
    const TVector& v;
    const TConstant c;

public:
    static constexpr unsigned int size = TVector::size;
    using data_type = typename TVector::data_type;
    TOperator op;

    VectorConstantOperation(
        const TVector& v,
        const TConstant c,
        TOperator op)
        : v(v)
        , c(c)
        , op(op)
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return op(v[index] , c);
    }
};

template <typename TLeft, typename TRight>
class VectorSum final{
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    VectorSum(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] + v2[index];
    }
};

template <typename TLeft, typename TRight>
class VectorProd final {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    VectorProd(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] * v2[index];
    }
};

template <typename TLeft, typename TRight>
class VectorMax final {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    VectorMax(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] < v2[index]
            ? v2[index]
            : v1[index];
    }
};

template <typename TVec>
constexpr auto operator-(const TVec& vec)
{
    return VectorNegative<TVec>(vec);
}

template<typename TData>
struct MulOperation{
    constexpr auto operator()(TData x,TData y) const
    {
        return x * y;
    }
};

template<typename TData>
struct AddOperation{
    constexpr auto operator()(TData x,TData y) const
    {
        return x + y;
    }
};

// vector + constant
template <
    typename TLeft,
    typename TRight,
    typename = typename TLeft::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
constexpr auto operator+(
    const TLeft& left,
    const TRight right)
{
    using data_type = typename TLeft::data_type;
    return VectorConstantOperation<TLeft,TRight,AddOperation<data_type>>
        (left,right,AddOperation<data_type>());
}

// constant + vector
template <
    typename TLeft,
    typename TRight,
    typename = typename TRight::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
constexpr auto operator+(
    const TLeft left,
    const TRight& right)
{
    using data_type = typename TRight::data_type;
    return VectorConstantOperation<TRight,TLeft,AddOperation<data_type>>
        (right,left,AddOperation<data_type>());
}

// vector * constant
template <
    typename TLeft,
    typename TRight,
    typename = typename TLeft::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
auto operator*(
    const TLeft& left,
    const TRight right)
{
    using data_type = typename TLeft::data_type;
    return VectorConstantOperation<TLeft,TRight,MulOperation<data_type>>
        (left,right,MulOperation<data_type>());
}

// constant * vector
template <
    typename TLeft,
    typename TRight,
    typename = typename TRight::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
constexpr auto operator*(
    const TLeft left,
    const TRight& right)
{
    using data_type = typename TRight::data_type;
    return VectorConstantOperation<TRight,TLeft,MulOperation<data_type>>
        (right,left,MulOperation<data_type>());
}

template <
    typename TLeft,
    typename TRight>
auto operator-(
    const TLeft& left,
    const TRight& right)
{
    return left + (-right);
}

template <
    typename TLeft,
    typename TRight,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if_t<sizeL==sizeR>
    >
constexpr auto operator+(
    const TLeft& left,
    const TRight& right)
{
    return VectorSum<TLeft, TRight>(left, right);
}

template <
    typename TLeft,
    typename TRight,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if_t<sizeL==sizeR>
    >
constexpr auto operator*(
    const TLeft& left,
    const TRight& right)
{
    return SUM(VectorProd<TLeft, TRight>(left, right));
}

class ToVector final {
};

template <
    typename TLeft,
    typename data_type = typename TLeft::data_type,
    unsigned int size = TLeft::size
    >
constexpr auto operator|(
    const TLeft& left,
    const ToVector& right)
{
    auto vec = Vector<size, data_type>();
    for (unsigned int i = 0; i < size; i++) {
        vec[i] = left[i];
    }

    return vec;
}

template <typename TLeft, typename TRight>
class VectorComponentMax final {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorComponentMax(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] > v2[index] ? v1[index] : v2[index];
    }
};

template <
    typename TLeft,
    typename TRight,
    unsigned int left_size = TLeft::size,
    unsigned int right_size = TRight::size,
    typename = typename std::enable_if_t<left_size==right_size>
    >
constexpr auto ComponentWiseMax(
    const TLeft& left,
    const TRight& right)
{
    return VectorComponentMax<TLeft, TRight>(left, right);
}

template <typename TVector>
constexpr auto Norm(TVector& v)
{
    return SUM(v * v);
}

}
